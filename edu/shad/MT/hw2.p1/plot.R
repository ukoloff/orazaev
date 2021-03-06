data = read.csv('timing.csv')
threads = unique(data$NumThreads)

time_mean = c()
time_min = c()
for (t in threads) {
    cur_thread_data = sort(data[data$NumThreads == t, "Time"])
    cur_thread_data = cur_thread_data[1:round(length(cur_thread_data) * 0.6)]

    time_mean = c(time_mean, mean(cur_thread_data))
    time_min = c(time_min, min(cur_thread_data))
}

png('from_1_to_40.png')
plot(threads, time_mean, type='l', xlab='число потоков', ylab='время в секундах', col='blue')
lines(threads, time_min, col='red')
dev.off()

one_thread_time = time_min[1]
png('acceleration.png')
plot(threads, time_min ^ -1 * one_thread_time, type='l', xlab='число потоков', ylab='ускорение', col='red')
lines(threads, time_mean ^ -1 * time_mean[1], type='l', col='blue')
dev.off()

png('efficiency.png')
plot(threads, time_min ^ -1 * one_thread_time / 1:length(time_min), type='l', xlab='число потоков', ylab='эффективность', col='red', ylim=c(0,1.4))
lines(threads, time_mean ^ -1 * time_mean[1] / 1:length(time_mean), type='l', col='blue')
dev.off()


cut_first = function(x, n=10) {
    return (x[n:length(x)])
}

print (sprintf('Лучшее по среднему значению число потоков: %d', threads[which.min(time_mean)]))

threads = cut_first(threads)
time_mean = cut_first(time_mean)
time_min = cut_first(time_min)

png('from_10_to_40.png')
plot(threads, time_mean, type='l', xlab='число потоков', ylab='время в секундах', col='blue', ylim=c(min(time_min), max(time_mean)))
lines(threads, time_min, col='red')
dev.off()
