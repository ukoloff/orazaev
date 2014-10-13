db.zips.aggregate([
    {$project: {
        first_char: {$substr: ["$city", 0, 1]},
        pop: 1
    }},
    {$match: {
        first_char: {$in: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '0']}
    }},
    {$group: {
        _id: null,
        total: {$sum: "$pop"}
    }}
])
