## ----setup, include=FALSE-----------------------------------------------------
# Also output all codes to a separate file.
knitr::knit_hooks$set(purl = knitr::hook_purl)
# Set global settings.
knitr::opts_chunk$set(echo = TRUE, tidy = TRUE)
# Load libraries.
library(tidyverse)
library(gridExtra)

## ---- echo=FALSE, message=FALSE-----------------------------------------------
dat <- read.csv(file = "../ssort.csv", col.names = c("size", "ops"))
ggplot(dat, aes(x = size, y = ops)) +
  geom_point() +
  geom_smooth()
summary(dat)

## ---- echo=FALSE--------------------------------------------------------------
fit <- lm(ops ~ poly(size, 10), data = dat)
summary(fit)

## ---- echo=FALSE--------------------------------------------------------------
fit <- lm(ops ~ size + I(size ^ 2), data = dat)
summary(fit)
confint(fit)
par(mfrow = c(2, 2))  # Split the plotting panel into a 2 x 2 grid
plot(fit)  # Plot the model information
par(mfrow = c(1, 1))  # Return plotting panel to 1 section
# Cite: https://drsimonj.svbtle.com/visualising-residuals

## ---- echo=FALSE--------------------------------------------------------------
fit <- lm(ops ~ floor(-6 + 5.5*size + 1.75*(size ^ 2)), data = dat)
summary(fit)
confint(fit)
par(mfrow = c(2, 2))
plot(fit)
par(mfrow = c(1, 1))

## ---- include=FALSE-----------------------------------------------------------
rm(dat, fit)

## ---- echo=FALSE, message=FALSE-----------------------------------------------
dat <- read.csv(file = "../pattern.csv", col.names = c("size", "ops"))
ggplot(dat, aes(x = size, y = ops)) +
  geom_point() +
  geom_smooth()
summary(dat)

## ---- echo=FALSE, message=FALSE-----------------------------------------------
tail(dat)

dat <- filter(dat, (ops >= 0) & (size < 1e8))
ggplot(dat, aes(x = size, y = ops)) +
  geom_point() +
  geom_smooth()

## ---- echo=FALSE--------------------------------------------------------------
fit <- lm(ops ~ size, data = dat)
summary(fit)
par(mfrow = c(2, 2))
plot(fit)
par(mfrow = c(1, 1))

## ---- echo=FALSE--------------------------------------------------------------
fit <- lm(ops ~ size * I(log2(size)), data = dat)
summary(fit)

## ---- echo=FALSE--------------------------------------------------------------
fit <- lm(ops ~ size + I(size * log2(size)), data = dat)
fit
confint(fit)
par(mfrow = c(2, 2))
plot(fit)
par(mfrow = c(1, 1))

## ---- include=FALSE-----------------------------------------------------------
rm(dat, fit)

## ---- echo=FALSE, message=FALSE-----------------------------------------------
dat <- read.csv(file = "../lsearch.csv", col.names = c("size", "ops"))
ggplot(dat, aes(x = size, y = ops)) +
  geom_point() +
  geom_smooth()
summary(dat)

## ---- echo=FALSE--------------------------------------------------------------
fit <- lm(ops ~ poly(size, 2) * I(2 ^ size), data = dat)
summary(fit)

## ---- echo=FALSE--------------------------------------------------------------
fit <- lm(ops ~ I(2 ^ size), data = dat)
fit
par(mfrow = c(2, 2))
plot(fit)
par(mfrow = c(1, 1))

## ---- include=FALSE-----------------------------------------------------------
rm(dat, fit)

## ---- echo=FALSE, message=FALSE-----------------------------------------------
dat <- read.csv(file = "../pow.csv", col.names = c("size", "ops"))
dat <-
  mutate(
    dat,
    oddexp = size %% 2 != 0,
    logSize = log2(size),
    logOps = log2(ops)
  )
summary(dat)
ggplot(filter(dat, size < 500), aes(x = size, y = ops, color = oddexp)) +
  geom_point() +
  geom_smooth()

## ---- echo=FALSE, message=FALSE-----------------------------------------------
ggplot(dat, aes(x = logSize, y = ops, color = oddexp)) +
  geom_point() +
  geom_smooth()

## ---- echo=FALSE, message=FALSE-----------------------------------------------
sodat <- read.csv(file = "../odd_pow.csv", col.names = c("size", "ops"))
sodat <-
  mutate(
    sodat,
    logSize = log2(size),
    logOps = log2(ops)
  )
head(sodat)
summary(sodat)
soplot <- ggplot(sodat, aes(x = logSize, y = ops)) +
  ggtitle("Small dataset (worst odds)") +
  geom_point() +
  geom_smooth()
oplot <- ggplot(filter(dat, oddexp == F), aes(x = logSize, y = ops)) +
  ggtitle("Full dataset (regular odds)") +
  geom_point() +
  geom_smooth()
grid.arrange(soplot, oplot, ncol = 2)
rm(soplot, oplot)

## ---- echo=FALSE--------------------------------------------------------------
sofit <- lm(ops ~ logSize, data = sodat)
summary(sofit)
confint(sofit)

## ---- echo=FALSE--------------------------------------------------------------
sedat <- read.csv(file = "../even_pow.csv", col.names = c("size", "ops"))
sedat <-
  mutate(
    sedat,
    logSize = log2(size),
    logOps = log2(ops)
  )
head(sedat)
sefit <- lm(ops ~ logSize, data = sedat)
sefit

## ---- echo=FALSE, message=FALSE-----------------------------------------------
newdat <- filter(dat, size < 1e3)
newdat <- 
  mutate(
    newdat,
    lowfit = predict(sefit, newdata = newdat),
    highfit = predict(sofit, newdata = newdat)
  )
ggplot(newdat, aes(x = size, y = ops, color = oddexp)) +
  geom_point() +
  geom_smooth(method = "lm",
              formula = y ~ log2(x)) +
  geom_line(data = newdat, aes(x = size, y = lowfit)) +
  geom_line(data = newdat, aes(x = size, y = highfit))

## ---- echo=FALSE, message=FALSE, fig.height = 4.65, fig.width = 7-------------
rm(sefit, sofit)
mfit.lo <- function(x) { 4*log2(x) + 5 }
mfit.hi <- function(x) { 5*log2(x) + 4 }
newdat$lowfit = mfit.lo(newdat$size)
newdat$highfit = mfit.hi(newdat$size)

ggplot(newdat, aes(x = size, y = ops, color = oddexp)) +
  ggtitle("Low: 4*log2(x)+5, High: 5*log2(x)+4") +
  geom_point() +
  geom_line(data = newdat, aes(x = size, y = lowfit)) +
  geom_line(data = newdat, aes(x = size, y = highfit))

## ---- echo=FALSE, message=FALSE, fig.height = 4.65, fig.width = 7-------------
snewdat <- filter(newdat, size < 30)
ggplot(snewdat, aes(x = size, y = ops, color = oddexp)) +
  ggtitle("Low: 4*log2(x)+5, High: 5*log2(x)+4") +
  geom_point() +
  geom_line(data = snewdat, aes(x = size, y = lowfit)) +
  geom_line(data = snewdat, aes(x = size, y = highfit))

rm(snewdat)

## ---- echo=FALSE, message=FALSE, fig.height = 4.65, fig.width = 7-------------
mfit.hi <- function(x) { 5*log2(x + 1) + 4 }
newdat$highfit = mfit.hi(newdat$size)

snewdat <- filter(newdat, size < 30)
ggplot(snewdat, aes(x = size, y = ops, color = oddexp)) +
  ggtitle("Low: 4*log2(x) + 5, High: 5*log2(x+1) + 4") +
  geom_point() +
  geom_line(data = snewdat, aes(x = size, y = lowfit)) +
  geom_line(data = snewdat, aes(x = size, y = highfit))

rm(snewdat)

## ---- echo=FALSE, message=FALSE, fig.height = 4.65, fig.width = 7-------------
ggplot(newdat, aes(x = size, y = ops, color = oddexp)) +
  ggtitle("Low: 4*log2(x) + 5, High: 5*log2(x+1) + 4") +
  geom_point() +
  geom_line(data = newdat, aes(x = size, y = lowfit)) +
  geom_line(data = newdat, aes(x = size, y = highfit))

## ---- echo=FALSE, message=FALSE, fig.height = 4.8, fig.width = 7--------------
newdat <- dat
newdat <- 
  mutate(
    newdat,
    lowfit = mfit.lo(size),
    highfit = mfit.hi(size)
  )
ggplot(newdat, aes(x = size, y = ops, color = oddexp)) +
  ggtitle("Low: 4*log2(x) + 5, High: 5*log2(x+1) + 4") +
  geom_point() +
  geom_line(data = newdat, aes(x = size, y = lowfit)) +
  geom_line(data = newdat, aes(x = size, y = highfit))

## ---- echo=FALSE, message=FALSE, fig.height = 4.8, fig.width = 7--------------
xdat <- read.csv(file = "../xpow.csv", col.names = c("size", "ops"))
xdat <-
  mutate(
    xdat,
    oddexp = size %% 2 != 0,
    logSize = log2(size),
    logOps = log2(ops),
    lowfit = mfit.lo(size),
    highfit = mfit.hi(size)
  )
ggplot(xdat, aes(x = size, y = ops, color = oddexp)) +
  ggtitle("Low: 4*log2(x) + 5, High: 5*log2(x+1) + 4") +
  geom_point() +
  geom_line(data = xdat, aes(x = size, y = lowfit)) +
  geom_line(data = xdat, aes(x = size, y = highfit))

## ---- echo=FALSE--------------------------------------------------------------
filter(newdat, (ops > highfit) | (ops < lowfit))
filter(xdat, (ops > highfit) | (ops < lowfit))

## ---- include=FALSE-----------------------------------------------------------
rm(mfit, newdat)

## ---- echo=FALSE--------------------------------------------------------------
read.csv(file = "../ssort.csv")[1:30,]

## ---- echo=FALSE--------------------------------------------------------------
head(read.csv(file = "../pattern.csv"), 30)

## ---- echo=FALSE--------------------------------------------------------------
head(unique(read.csv(file = "../lsearch.csv")), 30)

## ---- echo=FALSE--------------------------------------------------------------
head(dat, 30)
head(sodat, 20)
head(sedat, 20)
tail(xdat, 15)

