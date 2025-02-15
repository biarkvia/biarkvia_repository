db = db.getSiblingDB('db_songs');

db.createCollection("songs", {
    validator: {
        $jsonSchema: {
            bsonType: "object",
            required:  ["id_song", "id_label", "id_songwriter", "name_song", "release_date_song"],
            properties: {
                id_song: { bsonType: "number" },
                id_label: { bsonType: "number"},
                id_songwriter: { bsonType: "number"},
                name_song: { bsonType: "string"},
                release_date_song: { bsonType: "date"}
            }
        }
    }
});

db.createCollection("labels", {
    validator: {
        $jsonSchema: {
            bsonType: "object",
            required:  ["id_label", "name_label", "country_label"],
            properties: {
                id_label: { bsonType: "number"},
                name_labes: { bsonType: "string"},
                country_label: { bsonType: "string"}
            }
        }
    }
});

db.createCollection("songwriters", {
    validator: {
        $jsonSchema: {
            bsonType: "object",
            required:  ["id_songwriter", "username_songwriter", "full_name_songwriter", "age_songwriter"],
            properties: {
                id_songwriter: { bsonType: "number" },
                username_songwriter: { bsonType: "string"},
                full_name_songwriter: { bsonType: "string"},
                age_songwriter: { bsonType: "number"}
            }
        }
    }
});

sh.enableSharding("db_songs")
sh.shardCollection("db_songs.songs", {"id": 1});