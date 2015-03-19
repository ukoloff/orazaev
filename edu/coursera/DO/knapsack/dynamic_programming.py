# -*- coding: utf-8 -*-

def dynamic_programming_solution(capacity, item_count, items):
    dynamic_value = [[0] * (capacity + 1) for k in xrange(item_count + 1)]
    for i in xrange(1, item_count + 1):
        for k in xrange(1, capacity + 1):
            if k < items[i - 1].weight:
                dynamic_value[i][k] = dynamic_value[i - 1][k]
                #sys.stdout.write(str(dynamic_value[i][k]) + '\t')
                continue
            dynamic_value[i][k] = max(
                dynamic_value[i - 1][k],
                items[i - 1].value + dynamic_value[i - 1][k - items[i - 1].weight])
            #sys.stdout.write(str(dynamic_value[i][k]) + '\t')
        #print

    i = item_count
    k = capacity
    taken = [0] * item_count
    value = dynamic_value[i][k]
    while i > 0 and k > 0 and dynamic_value[i][k] > 0:
        while dynamic_value[i][k] == dynamic_value[i - 1][k]:
            i -= 1
        while dynamic_value[i][k] == dynamic_value[i][k - 1]:
            k -= 1
        taken[i - 1] = 1
        i -= 1
        k -= items[i].weight

    return value, taken
