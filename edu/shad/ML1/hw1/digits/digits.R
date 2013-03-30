set.seed(42)
#digitsData = datasets::iris
#names(digitsData) = c("f1", "f2", "f3", "f4", "y")
load('digits_train_data.RData')

#TRAIN_PERCENT = 0.001
TRAIN_PERCENT = 0.001

source('em.R')

testDigitsData = data.frame()

GetNormalizer = function(X) {
  normalizer = list()

  means = colMeans(X)
  vars = sqrt(apply(t(X), 1, var))

  normalizer$Function = function(x) { return((x - means) / vars) }
  normalizer$NonZeroVars = vars != 0

  return (normalizer)
}

GetClassifier = function(X) {
  classifier = list()
  classifier$Thetas = list()

  y = X[,ncol(X)]
  X = X[, -ncol(X)]
  
  normalizer = GetNormalizer(X)
  X = t(apply(X, 1, normalizer$Function))[, normalizer$NonZeroVars]
  classifier$Normalizer = normalizer

  for (currentLabel in unique(y)) {
    print(sprintf("Train classifier for label %s", currentLabel))
    labelData = X[y == currentLabel, ]

    classifier$Thetas[[length(classifier$Thetas) + 1]] =
        GEM(as.matrix(labelData), 0.001)

    classifier$Thetas[[length(classifier$Thetas)]]$Label = currentLabel
  }

  return(classifier)
}

Classify = function(classifier, X) {
  X = as.matrix(X)

  # Normalization
  normalizer = classifier$Normalizer
  X = t(apply(X, 1, normalizer$Function))[, normalizer$NonZeroVars]

  Den = NA
  for (Theta in classifier$Thetas) {
    current = apply(X, 1, function(x) { return (CalcDencity(x, Theta)) })

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
save(classifier, file="classifier.RData")

res = Classify(classifier, testData[, -ncol(testData)])
