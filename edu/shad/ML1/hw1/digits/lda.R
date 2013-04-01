set.seed(242)
source('common.R')

load('digits_train_data.RData')

data = prepareData(digitsData, train.factor=1)
lda = trainLda(data$train)


#res = lda$Classify(data$test[, -ncol(data$test)])
#error = res != data$test[, ncol(data$test)]

#score = 1 - (sum(error) / nrow(data$test))

#print(sprintf("Classifier accuracy: %f", score))
