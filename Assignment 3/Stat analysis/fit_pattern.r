library(tidyverse)

dat <- read.csv(file = "pattern.csv", col.names = c("size", "ops"))
head(dat)

# Remove overflowed integers.
# Compiled with -fwrapv on gcc.
dat <- filter(dat, ops >= 0)
head(dat)
summary(dat)

ggplot(dat, aes(x = size, y = ops)) +
  geom_point() +
  geom_smooth()

# Zoom in at the expense of losing data.
ggplot(filter(dat, size < 1e8), aes(x = size, y = ops)) +
  geom_point() +
  geom_smooth()
# This is way better.

# Graph looks linear.
# Trying to fit a simple line.
fit <- lm(ops ~ size, data = dat)
summary(fit)
plot(fit, which = 1)
plot(fit, which = 5)
# Doesn't look very good.

# Remove outlier #30 and try again.
dat <- dat[-30, ]
fit <- lm(ops ~ size, data = dat)
summary(fit)
plot(fit, which = 1)
plot(fit, which = 5)

# Plots and coefficients are still weird, try filtering and add terms.
# Adding log as algorithm apparently halves n on each recursion.
dat <- filter(dat, size < 1e8)
fit <- lm(ops ~ size * I(log2(size)), data = dat)
summary(fit)
plot(fit, which = 1)
plot(fit, which = 5)
# Plots look better.
# Model fits perfectly.
# Coefficients are reasonable.


# Try a different approach.
# Log transform.
newdat <- mutate(dat, logSize = log2(size), logOps = log2(ops))
newdat <- filter(newdat, logSize < 25)
ggplot(newdat, aes(x = logSize, y = logOps)) +
  geom_point() +
  geom_smooth()
