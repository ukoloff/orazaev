qdaLearn = function(train_data, label_column_name) {
  label_column_index = which(names(train_data) == label_column_name)
  class_labels = unique(train_data[,label_column_name])
  
  priors = vector(length=length(class_labels))
  mean_estimates = list(length=length(class_labels))
  var_estimates = list(length=length(class_labels))

  total_objects_count = nrow(train_data)

  for (label in 1:length(class_labels)) {
    class_data = train_data[train_data[[label_column_name]]==class_labels[label],]

    priors[label] = nrow(class_data) / total_objects_count
    mean_estimates[[label]] = sapply(class_data[,-label_column_index], mean)
    var_estimates[[label]] = var(class_data[,-label_column_index])
  }

  classifier = list(
    Priors=priors,
    Means=mean_estimates,
    dVars=sapply(var_estimates, det),   # determinants
    iVars=lapply(var_estimates, solve), # inverted matrices
    ClassNames=class_labels)

  return(classifier)
}

qdaClassify = function(classifier, x) {
  x = as.matrix(x)
  class_sizes = length(classifier$Means)
  confidences = vector(length=class_sizes)
  number_of_features = length(x)

  for (i in 1:length(confidences)) {
    residual = x - classifier$Means[[i]]
    confidences[i] = log(classifier$Priors[i]) -
        (t(residual) %*% classifier$iVars[[i]] %*% residual) / 2 -
        log(classifier$dVars[i]) / 2
  }
  max_confidence_index = which(confidences == max(confidences))

  return(classifier$ClassNames[max_confidence_index])
}

set.seed(13)
random_indices_permutation = sample(1:nrow(iris))
train_objects_indices = random_indices_permutation[1:(nrow(iris)/2)]
test_objects_indices = random_indices_permutation[(nrow(iris)/2+1):nrow(iris)]

train_iris = iris[train_objects_indices,]
test_iris = iris[test_objects_indices,]

qda_classifier = qdaLearn(train_iris, "Species")
predictions = apply(test_iris[,1:4], 1, qdaClassify, classifier=qda_classifier)
error = length(which(predictions != test_iris$Species)) / length(predictions)
print(sprintf("Error of my qda classifier = %f", error))

library("MASS")
qda_classifier_from_box = qda(train_iris[,1:4], train_iris[,5])
predictions_from_box = predict(qda_classifier_from_box, test_iris[,1:4])$class
error_from_box = length(which(predictions_from_box != test_iris$Species)) / length(predictions)
print(sprintf("Error of MASS::qda classifier = %f", error))
