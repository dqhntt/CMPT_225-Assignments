library(tidyverse)

dat <- read.csv(file = "pow.csv", col.names = c("size", "ops"))
head(dat)
summary(dat)

ggplot(dat, aes(x = size, y = ops)) +
  geom_point() +
  geom_smooth()

# Add var showing odd exponents.
dat <-
  mutate(
    dat,
    oddexp = size %% 2 != 0,
    logSize = log2(size),
    logOps = log2(ops)
  )
ggplot(dat, aes(x = size, y = ops, color = oddexp)) +
  geom_point() +
  geom_smooth()

# Zooming in for fun.
ggplot(filter(dat, size < 1e3), aes(x = size, y = ops, color = oddexp)) +
  geom_point() +
  geom_smooth()

# Log transform x.
ggplot(dat, aes(x = logSize, y = logOps, color = oddexp)) +
  geom_point() +
  geom_smooth()

# Clear that odd exp have higher ops count.
ggplot(dat, aes(x = logSize, y = ops, color = oddexp)) +
  geom_point() +
  geom_smooth()

# Separate.
odat <- filter(dat, oddexp == T)
edat <- filter(dat, oddexp == F)

ggplot(odat, aes(x = size, y = ops, color = oddexp)) +
  geom_point() +
  geom_smooth()

ggplot(edat, aes(x = size, y = ops, color = oddexp)) +
  geom_point() +
  geom_smooth()
rm(edat)

# Fit odd data only, more interested in upper bound.
# Not sure log or nlog, try both.
fit <- lm(ops ~ size * I(log2(size)), data = odat)
summary(fit)

# Only log n seems reasonable.
# Checking if log or poly of log.
fit <- lm(ops ~ poly(logSize, 2), data = odat)
summary(fit)

# Only log n.
fit <- lm(ops ~ logSize, data = odat)
summary(fit)
plot(fit, which = 1)
plot(fit, which = 5)

# Trying to overlay fitted model.
ggplot(odat, aes(x = size, y = ops, color = oddexp)) +
  geom_point() +
  geom_smooth(method = "lm", formula = y ~ log2(x))

# Wanting an upper bound only.
# Supplying own model.
mfit <- function(x) {
  5 * log2(x) + 3
}

# Make new dataset with that model.
newdat <- dat
#newdat$ops <- predict(fit, newdata = newdat)
newdat$ops <- mfit(dat$size)
ggplot(filter(newdat, size < 1e3), aes(x = size, y = ops, color = oddexp)) +
  geom_point()

# Trying to overlay data with new model.
ggplot(filter(dat, size < 20), aes(x = size, y = ops, color = oddexp)) +
  geom_point() +
  geom_smooth(method = "lm",
              formula = y ~ log2(x),
              colour = "red") +
  geom_line(data = filter(newdat, size < 20), aes(x = size)) + 
  geom_line(data = filter(newlowdat, size < 20), aes(x = size))

# Zoom out.
ggplot(dat, aes(x = size, y = ops, color = oddexp)) +
  geom_point() +
  geom_smooth(method = "lm",
              formula = y ~ log2(x),
              colour = "red") +
  geom_line(data = newdat, aes(x = size))

# Looks decent, now trying with more extreme dataset.
xdat <- read.csv(file = "xpow.csv", col.names = c("size", "ops"))
xdat <-
  mutate(
    xdat,
    oddexp = size %% 2 != 0,
    logSize = log2(size),
    logOps = log2(ops)
  )
summary(xdat)

# Overlay own model.
xnewdat <- xdat
mfit <- function(x) { 5*log2(x) + 4 }
xnewdat$ops <- mfit(xdat$size)
xnewlowdat <- xdat
mfit <- function(x) { 4*log2(x) + 5 }
xnewlowdat$ops <- mfit(xdat$size)
ggplot(xdat, aes(x = size, y = ops, color = oddexp)) +
  geom_point() +
  geom_line(data = xnewdat, aes(x = size)) + 
  geom_line(data = xnewlowdat, aes(x = size))

# Trying to fit a sin cos model.
fit <- lm(ops ~ log2(size) * sin(2*pi*size), data = dat)
summary(fit)
plot(fit, which = 1)
plot(fit, which = 5)

ggplot(xdat, aes(x = size, y = ops, color = oddexp)) +
  geom_point() +
  geom_line(data = xnewdat, aes(x = size))


