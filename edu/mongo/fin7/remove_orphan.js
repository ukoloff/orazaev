use final7;
var images = db.images;
var albums = db.albums;

var imagesCursor = images.find();

print("=== Searching orphans...");

var count = 0;

while (imagesCursor.hasNext()) {
    var img = imagesCursor.next();
    var firstAlbum = albums.findOne({'images': img._id});

    if (firstAlbum === null) {
        count += 1;
        images.remove({_id: img._id});
    }
}

print("=== Removed: " + count.toString());
