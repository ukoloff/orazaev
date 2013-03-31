source('common.R')

load('digits_train_data.RData')

X = digitsData[digitsData$y == 2, ]
LOADING_PERCENT = 0.6
X = X[1:(nrow(X) * LOADING_PERCENT), ]

Y = X[, ncol(X)]
X = X[, -ncol(X)]


print(sprintf("Number of objects = %d", nrow(X)))

## Preprocessing X
#Z = X
preprocessing = getPreprocessing(X)
norm.X = preprocessing$Do(X)


Theta = list()
Theta$Mean = colMeans(norm.X)
Theta$Sigma = colVars(norm.X)

print(calcMvnormDensity(norm.X[1,], Theta$Mean, Theta$Sigma))
print(prod(calcNativeBayesDensity(norm.X[1,], Theta$Mean, Theta$Sigma)))

source('em.R')
Theta = GEM(norm.X, 0.001)

x = preprocessing$Do(digitsData[digitsData$y == 2,][4000,-ncol(digitsData)])
print(CalcDensity(x, Theta))

x = preprocessing$Do(digitsData[digitsData$y == 1,][1,-ncol(digitsData)])
print(CalcDensity(x, Theta))
