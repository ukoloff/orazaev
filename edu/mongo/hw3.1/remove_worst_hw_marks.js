use school;
var students = db.students;

var cursor = students.find();

while (cursor.hasNext()) {
    var student = cursor.next();
    var worstHomeworkIndex = null;
    for (var i = 0; i < student.scores.length; ++i) {
        if (student.scores[i].type != "homework") { continue; }

        if (worstHomeworkIndex === null) {
            worstHomeworkIndex = i;
            continue;
        }

        if (worstHomeworkIndex === null
            || student.scores[worstHomeworkIndex].score > student.scores[i].score)
        {
            worstHomeworkIndex = i;
        }
    }

    student.scores.splice(worstHomeworkIndex, 1);
    students.save(student);
}
