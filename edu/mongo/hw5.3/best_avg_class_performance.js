use test;

db.grades.aggregate([
    {$unwind: "$scores"},
    {$match: {
        "scores.type": {$ne: "quiz"}
    }},
    {$group: {
        _id: "$class_id",
        avg_scores: {$avg: "$scores.score"}
    }},
    {$sort: {avg_scores: -1}}
]);
