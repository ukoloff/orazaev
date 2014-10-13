use test;

db.zips.aggregate([
    {$group: {
        _id: {state: "$state", city: "$city"},
        pop: {$sum: "$pop"}
    }},
    {$match: {
        pop: {$gt: 25000},
        "_id.state": {$in: ["NY", "CA"]}
    }},
    {$group: {
        _id: null,
        avg_pop: {$avg: "$pop"}
    }}
]);
