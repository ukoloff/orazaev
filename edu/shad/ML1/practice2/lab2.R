# add here commands for installation of packages "mvtnorm" and "np"
# ...
library('mvtnorm')
library('np')

# fixed seed for results reproducibility
set.seed(13)

# dimensionality of data
dimension = 2

# constant normal distribution params
MEAN = numeric(dimension)
SIGMA = diag(dimension)

# bws for npudens
BWS = numeric(dimension) + 0.5;

# fixed number of evaluation points
eval_points_number = 1000

# generate evaluation points
eval_data = rmvnorm(eval_points_number, mean=MEAN, sigma=SIGMA)

getDensityEstimationError = function(train_points_number) {

# generate train points
train_data = rmvnorm(train_points_number, mean=MEAN, sigma=SIGMA)

# train density estimator
density_estimator = npudens(bws=BWS, train_data)

# predict density values in evaluation points
predicted_values = predict(density_estimator, edat=eval_data)

# calculate groundtruth density values in evaluation points
groundtruth_values = dmvnorm(eval_data, mean=MEAN, sigma=SIGMA)

# calculate mean relative square error
mean_relative_square_error = mean(
        ((groundtruth_values - predicted_values) ^ 2) / (groundtruth_values ^ 2)
)

return (mean_relative_square_error)
}

counts = 2 ^ seq(7, 12, by=0.3)
counts = as.integer(counts)

print(counts)
errors = sapply(counts, getDensityEstimationError)
print(errors)
smoothed_errors = smooth(errors)
plot(counts, errors)

# Function to calculate minimal traing set size
getMinimalTrainingDataSizeToBeatErrorThreshold = function(threashold=0.05) {
    trainingDataSize = 1
    meanError = getDensityEstimationError(trainingDataSize)
    while (meanError > threashold) {
        print(sprintf("Size: %d, Error: %f", trainingDataSize, meanError))
        trainingDataSize = trainingDataSize + 1
        meanError = getDensityEstimationError(trainingDataSize)
    }
    print(sprintf("Size: %d, Error: %f", trainingDataSize, meanError))

    return(trainingDataSize)
}

getMinimalTrainingDataSizeToBeatErrorThreshold()
