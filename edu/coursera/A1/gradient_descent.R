data <- read.csv('data.csv')
m <- length(data$X)


CalculateError <- function(theta) {
    result <- sum((data$Y - theta[1] * data$X ^ theta[2]) ^ 2) / (2 * m)

    return(result)
}


GetNewTheta <- function(theta, alpha=0.1) {
    newTheta <- c(0, 0)

    newTheta[1] <-
        theta[1] + sum(alpha * (data$Y - theta[1] * data$X ^ theta[2]) *
        (data$X ^ theta[2]) / m)

    print(sum(alpha * (data$Y - theta[1] * data$X ^ theta[2]) *
        (data$X ^ theta[2]) / m))

    newTheta[2] <-
        theta[2] + sum(alpha * (data$Y - theta[1] * data$X ^ theta[2]) *
        (theta[1] * data$X ^ theta[2] * log(data$X)) / m)

    print(sum(alpha * (data$Y - theta[1] * data$X ^ theta[2]) *
        (theta[1] * data$X ^ theta[2] * log(data$X)) / m))

    return(newTheta)
}


StartGradientDescent <- function(numberOfSteps, alpha=0.001) {
    errorVector <- vector(mode='numeric', length=numberOfSteps + 1)

    theta <- c(1, 1)
    errorVector[1] <- CalculateError(theta)

    for(i in 1:numberOfSteps) {
        theta <- GetNewTheta(theta, alpha)

        errorVector[i + 1] <- CalculateError(theta)
    }

    plot(errorVector, pch=19)
    return(theta)
}


StartGradientDescent(10)
