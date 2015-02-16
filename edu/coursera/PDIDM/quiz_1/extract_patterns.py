#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import itertools

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('TRANSACTIONS_TSV')
    parser.add_argument('-s', '--size', type=int, default=2)
    return parser.parse_args()

def calc_support(itemset, base):
    count = 0
    for values in base.values():
        if itemset & values == itemset:
            count += 1
    return float(count) / len(base)

def mine_support(transactions, sz):
    results = {}
    for values in transactions.values():
        for itemset in itertools.combinations(values, sz):
            if itemset not in results:
                results[tuple(sorted(itemset))] = calc_support(
                    set(itemset), transactions)
    return results

def mine_association_rules(transactions):
    items_support = mine_support(transactions, 1)
    paris_support = mine_support(transactions, 2)
    results = {}
    for pair, pair_support in paris_support.iteritems():
        results['{0} -> {1}'.format(*pair)] = (
            pair_support,
            pair_support / items_support[(pair[0],)])
        results['{1} -> {0}'.format(*pair)] = (
            pair_support,
            pair_support / items_support[(pair[1],)])
    return results

def main(args):
    transactions = {}

    with open(args.TRANSACTIONS_TSV) as f:
        for line in f:
            tid, items = line.split('\t')
            transactions[int(tid)] = set([
                i.strip().lower() for i in items.split(',')])

    results = mine_support(transactions, args.size)
    for itemset, support in sorted((v, k) for k, v in results.iteritems()):
        print support, itemset
    print '-'*10
    for stats, rule in sorted((v, k) for k, v in mine_association_rules(transactions).items()):
        print stats, rule

if __name__ == '__main__':
    main(parse_args())
