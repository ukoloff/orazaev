set.seed(42)
#digitsData = datasets::iris
#names(digitsData) = c("f1", "f2", "f3", "f4", "y")
digitsData = read.csv('train.csv')

TRAIN_PERCENT = 0.001

source('em.R')

testDigitsData = data.frame()

GetClassifier = function(X) {
  classifier = list()

  for (currentLabel in unique(X$y)) {
    print(sprintf("Train classifier for label %s", currentLabel))
    labelData = X[X$y == currentLabel, ]

    classifier[[length(classifier) + 1]] =
        GEM(as.matrix(labelData[, -ncol(labelData)]), 0.001)

    classifier[[length(classifier)]]$Label = currentLabel
  }

  return(classifier)
}

Classify = function(classifier, X) {
  X = as.matrix(X)
  Den = NA
  for (Theta in classifier) {
    current = apply(X, 1, function(x) { return (CalcDencity(x, Theta)) })

    if (is.na(Den)) {
      Den = current
    } else {
      Den = rbind(Den, current)
    }
  }

  return (apply(t(Den), 1, function(x) { return (classifier[[which.max(x)]]$Label) }))
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
