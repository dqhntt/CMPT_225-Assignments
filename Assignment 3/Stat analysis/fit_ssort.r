library(tidyverse)

dat <- read.csv(file = "ssort.csv", col.names = c("size", "ops"))
head(dat)
summary(dat)

ggplot(dat, aes(x = size, y = ops)) +
  geom_point() +
  geom_smooth()

fit <- lm(ops ~ size * I(size ^ 2), data = dat)
summary(fit)

# The last term is insignificant -> reduce to 3 terms.
fit <- lm(ops ~ size + I(size ^ 2), data = dat)
summary(fit)
plot(fit, which = 1)
plot(fit, which = 2)

# Do some rounding with the fitted coefficients.
fit <- lm(ops ~ floor(-6 + 5.5*size + 1.75*(size ^ 2)), data = dat)
summary(fit)
plot(fit, which = 1)
plot(fit, which = 2)

# And the orthogonal polynomial that I still don't understand yet.
fit <- lm(ops ~ poly(size, 2), data = dat)
summary(fit)
