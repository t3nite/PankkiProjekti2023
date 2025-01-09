// Moduulien lataus
const db = require("../config/db.js"); // Tietokantayhteyden muodostamiseen tarvittava moduuli
const bcrypt = require("bcrypt"); // Salasanan hajautusta varten tarvittava moduuli

// Salmiakierrosten määrä bcrypt:in käyttöön
const saltRounds = 10;

// Käyttäjä-olio, joka sisältää erilaisia tietokantakyselyjä käyttäjän hallintaan
const user = {
    // Hakee kaikki käyttäjät
    get: function(callback){
        return db.query("SELECT * FROM user", callback);
    },
    // Hakee käyttäjän ID:n perusteella
    getById: function(id, callback){
        return db.query("SELECT * FROM user WHERE iduser = ?", [id], callback);
    },
    // Hakee käyttäjän sähköpostiosoitteen perusteella
    getByEmail: function(email, callback){
        return db.query("SELECT * FROM user WHERE email = ?", [email], callback);
    },
    // Hakee käyttäjän tunnistetiedot (sähköposti ja salasana) perusteella
    getCredentials: function(email, password, callback){
        return db.query("SELECT email, pword FROM user WHERE email = ? AND pword = ?",
        [email, password], callback);
    },
    // Lisää uuden käyttäjän tietokantaan
    add: function(newuser, callback){
        // Salasanan hajautus bcrypt:in avulla
        bcrypt.hash(newuser.pword, saltRounds, function(err, hash){
            // Lisätään käyttäjä tietokantaan hashatulla salasanalla
            return db.query("INSERT INTO user(email, pword, fname, lname, adress, phonenumber, picture, username,iduser) VALUES(?,?,?,?,?,?,?,?,?)",
            [newuser.email, hash, newuser.fname, newuser.lname, newuser.adress, newuser.phonenumber,newuser.picture,newuser.username, newuser.iduser], callback);
        });
    },
    // Poistaa käyttäjän tietokannasta ID:n perusteella
    deleteuser: function(iduser, callback){
        return db.query("DELETE FROM user WHERE iduser = ?", [iduser], callback);
    },
    // Päivittää käyttäjän profiilikuvan tietokantaan
    updateAvatar: function(iduser, file, callback){
        return db.query("UPDATE user SET picture = ? WHERE iduser = ?",
        [file, iduser], callback);
    }
};

module.exports = user; // Vienti, jotta user-olio voidaan käyttää muualla sovelluksessa
