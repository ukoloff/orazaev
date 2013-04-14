data = read.csv('timing.csv')
threads = c(0:228)
processors = unique(data$NumProcessors)
nodes = unique(data$NumNodes)
data_threads = data$NumNodes * data$NumProcessors

time_mean = c()
time_min = c()
for (t in threads) {
    cur_thread_data = sort(data[data_threads == t, "Time"])
    cur_thread_data = cur_thread_data[1:round(length(cur_thread_data) * 0.8)]

    time_mean = c(time_mean, mean(cur_thread_data))
    time_min = c(time_min, min(cur_thread_data))
}

png('all_values.png')
plot(threads[!is.na(time_mean)] , time_mean[!is.na(time_mean)], type='l', xlab='число потоков', ylab='время в секундах', col='blue')
lines(threads[!is.na(time_mean)], time_min[!is.na(time_mean)], col='red')
dev.off()

png('acceleration.png')
plot(threads[!is.na(time_mean)] , time_mean[1] / time_mean[!is.na(time_mean)], type='l', xlab='число потоков', ylab='ускорение', col='blue')
lines(threads[!is.na(time_mean)], time_min[!is.na(time_mean)] ^ -1 * time_min[1], col='red')
dev.off()

cur_threads = threads[!is.na(time_mean)]
cur_threads[1] = 1
png('efficiency.png')
plot(threads[!is.na(time_mean)] , time_mean[1] / (time_mean[!is.na(time_mean)] * cur_threads), type='l', xlab='число потоков', ylab='эффективность', col='blue')
lines(threads[!is.na(time_mean)], time_min[!is.na(time_mean)] ^ -1 * time_min[1] / cur_threads, col='red')
dev.off()

cut_first = function(x, n=30) {
    return (x[n:length(x)])
}

print (sprintf('Лучшее по среднему значению число потоков: %d', threads[which.min(time_mean)]))

threads = threads[!is.na(time_mean)]
time_min = time_min[!is.na(time_mean)]
time_mean = time_mean[!is.na(time_mean)]

threads = cut_first(threads)
time_mean = cut_first(time_mean)
time_min = cut_first(time_min)

png('fast_values.png')
plot(threads, time_mean, type='l', xlab='число потоков', ylab='время в секундах', col='blue', ylim=c(min(time_min), max(time_mean)))
lines(threads, time_min, col='red')
dev.off()


time_mean = matrix(nrow=length(nodes), ncol=length(processors) - 1)
nodes = cut_first(nodes, 1)
processors = cut_first(processors, 2)

for (n in nodes) {
    for (p in processors) {
        cur_data = sort(data[(data$NumNodes == n & data$NumProcessors == p) , "Time"])
        cur_data = cur_data[1:round(length(cur_data) * 0.8)]
        
        time_mean[n, p] = mean(cur_data)
    }
}

for (p in processors) {
    time_mean[4, p] = (time_mean[3, p] + time_mean[5, p]) / 2
}

nodes = sort(c(nodes, 4))
nodes = cut_first(nodes, 2)

png('plot_in_3d.png')
persp(nodes, processors, time_mean, theta=120, xlab="Число узлов", ylab="Число Процессоров", zlab="Среднее время в секундах")
dev.off()

nodes = cut_first(nodes, 5)
processors = cut_first(processors, 5)

time_mean = time_mean[5:nrow(time_mean), 5:ncol(time_mean)]

#persp(nodes, processors, time_mean, theta=45, xlab="Число узлов", ylab="Число Процессоров", zlab="Среднее время в секундах")
