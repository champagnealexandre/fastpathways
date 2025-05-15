# Load libraries
library(ggplot2)
library(dplyr)
library(readr)
library(splines)

# Load data
df <- read_csv("assembly_indices.csv", show_col_types = FALSE)

# Clean numeric columns
df <- df %>%
  mutate(
    integer = as.integer(integer),
    calc_time = as.numeric(calc_time)
  ) %>%
  filter(!is.na(calc_time))

# Plot: calc_time vs integer
ggplot(df, aes(x = integer, y = calc_time)) +
  geom_point(alpha = 0.5) +
  geom_smooth(method = "lm", formula = y ~ ns(x, 3), se = FALSE, color = "blue") +
  labs(title = "Computation Time per Integer",
       x = "Integer",
       y = "Time (seconds)") +
  theme_minimal()

# Fit a spline regression model
model <- lm(calc_time ~ ns(integer, 3), data = df)

# Function to predict time for one integer
estimate_time <- function(x) {
  predict(model, newdata = data.frame(integer = x))
}

# Function to predict total time from 1 to x
estimate_total_time <- function(x) {
  integers <- 1:x
  sum(estimate_time(integers))
}

# Example usage
X <- 100000
time_for_X <- estimate_time(X)
total_time_to_X <- estimate_total_time(X)

cat(sprintf("Estimated time to compute integer %d: %.3f seconds\n", X, time_for_X))
cat(sprintf("Estimated total time for integers 1 to %d: %.3f seconds\n", X, total_time_to_X))