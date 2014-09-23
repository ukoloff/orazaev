#!/usr/bin/env python

import pymongo



def remove_lowest_homework():
    connection = pymongo.MongoClient('localhost', 27017)
    students = connection.students
    grades = students.grades

    cursor = grades.find({'type': 'homework'})
    cursor = cursor.sort([
        ('student_id', pymongo.ASCENDING),
        ('score', pymongo.ASCENDING)])

    previous_student_id = None
    for number, item in enumerate(cursor):
        if previous_student_id == item['student_id']:
            continue
        previous_student_id = item['student_id']
        grades.remove(item)


remove_lowest_homework()
