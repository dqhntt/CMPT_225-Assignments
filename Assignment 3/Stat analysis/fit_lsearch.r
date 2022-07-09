library(tidyverse)

dat <- read.csv(file = "lsearch.csv", col.names = c("size", "ops"))
head(dat)
summary(dat)

ggplot(dat, aes(x = size, y = ops)) +
  geom_point() +
  geom_smooth()

# Not sure exponential or polynomial, try both.
fit <- lm(ops ~ poly(size, 2) * I(2 ^ size), data = dat)
summary(fit)

# Last 2 terms are insignificant -> reduce to 4 terms.
fit <- lm(ops ~ poly(size, 2) + I(2 ^ size), data = dat)
summary(fit)

# Looks pretty good, try again with regular polynomials.
fit <- lm(ops ~ I(2 ^ size), data = dat)
summary(fit)
plot(fit, which = 1)
plot(fit, which = 5)
