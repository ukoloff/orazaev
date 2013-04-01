set.seed(42)

source('common.R')

#digitsData = datasets::iris
#names(digitsData) = c("f1", "f2", "f3", "f4", "y")
load('digits_train_data.RData')

TRAIN_PERCENT = 1

source('em.R')

testDigitsData = data.frame()

GetNormalizer = function(X) {
  normalizer = list()

  means = as.vector(colMeans(X))
  vars = sqrt(as.vector(colVars(X)))

  normalizer$Function = function(x) { return((x - means) / vars) }
  normalizer$NonZeroVars = vars > 0.001 ^ 2

  return (normalizer)
}

GetClassifier = function(X) {
  classifier = list()
  classifier$Thetas = list()

  y = X[,ncol(X)]
  X = X[, -ncol(X)]

  for (currentLabel in unique(y)) {
    print(sprintf("Train classifier for label %s", currentLabel))
    labelData = X[y == currentLabel, ]

    #normalizer = GetNormalizer(labelData)
    #labelData = t(apply(labelData, 1, normalizer$Function))[, normalizer$NonZeroVars]
    preprocessing = getPreprocessing(labelData)
    labelData = preprocessing$Do(labelData)

    numClusters = 10
    if (currentLabel == 2 || currentLabel == 3) {
        numClusters = 0
    }
    classifier$Thetas[[length(classifier$Thetas) + 1]] =
        GEM(as.matrix(labelData), 0.005)

    classifier$Thetas[[length(classifier$Thetas)]]$Preprocessing = preprocessing
    classifier$Thetas[[length(classifier$Thetas)]]$Label = currentLabel
  }

  return(classifier)
}

Classify = function(classifier, X) {
  X = as.matrix(X)

  # Normalization
  #normalizer = classifier$Normalizer
  #X = t(apply(X, 1, normalizer$Function))[, normalizer$NonZeroVars]

  Den = NA
  for (Theta in classifier$Thetas) {
    #curX = t(apply(X, 1, Theta$Normalizer$Function))[,Theta$Normalizer$NonZeroVars]
    curX = Theta$Preprocessing$Do(X)

    current = apply(curX, 1, function(x) { return (CalcDensity(x, Theta)) })

    if (any(is.na(Den))) {
      Den = current
    } else {
      Den = rbind(Den, current)
    }
  }

  return (apply(t(Den), 1, function(x) { return (classifier$Thetas[[which.max(x)]]$Label) }))
}

trainData = data.frame();
testData = data.frame();

for (currentLabel in unique(digitsData$y)) {
  labelData = digitsData[digitsData$y == currentLabel, ]
  currentSize = nrow(labelData)

  currentSample = labelData[sample(1:currentSize, currentSize),]
  trainSize = round(currentSize * TRAIN_PERCENT)
  trainData = rbind(trainData, currentSample[1:trainSize,])
  testData = rbind(testData, currentSample[(trainSize + 1):currentSize,])
}


classifier = GetClassifier(trainData)
#save(classifier, file="classifier.RData")
testData = read.csv('test.csv')

res = Classify(classifier, testData[, -ncol(testData)])
