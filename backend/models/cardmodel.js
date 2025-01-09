// Tarvittavien moduulien lataus
const db = require("../config/db.js"); // Moduuli tietokantayhteyden hallintaan
const bcrypt = require("bcrypt"); // Moduuli salasanan hashaukseen

// Salmakierrosten määrä hashauksessa
const saltRounds = 10;

// Määritellään "card" objekti, joka sisältää erilaisia tietokantakyselyitä kortteihin liittyen
const card = {
    // Hakee kaikki kortit tietokannasta
    get: function(callback){
        return db.query("SELECT * FROM card", callback);
    },
    // Hakee kortin numeron ja PIN-koodin tietokannasta tietyn kortin id:n perusteella
    getcardnumberpin: function(callback){
        return db.query("select card_number, pin FROM card where idcard = 4;", callback);
    },

    // Lisää uuden kortin tietokantaan
    addCard: function(newcard,callback){
        console.log("model"); // Tulostaa konsoliin "model" tekstin
        // Hashataan uuden kortin PIN-koodi ennen lisäämistä tietokantaan
        bcrypt.hash(newcard.pin, saltRounds, function(err, hash){
            return db.query("INSERT INTO card (pin, card_number, active,idcard,idaccount,user_iduser,cardtype ) VALUES(?,?,?,?,?,?,?)",
            [hash,newcard.card_number,newcard.active,newcard.idcard,newcard.idaccount,newcard.user_iduser,newcard.cardtype,], callback);
        });
    },

    // Lisää kortin tiettyyn käyttäjätiliin
    addCardToAccount: function(user_idcard, idaccount, callback){
        return db.query("INSERT INTO (user_iduser, idaccount) VALUES(?, ?)", [iduser, idaccount], callback);
    },

    // Hakee kortin tietyn numeron perusteella
    getByNumber: function(card_number, callback){
        return db.query("SELECT * FROM card WHERE card_number = ?", [card_number], callback);
    },

    // Hakee käyttäjän kaikki kortit tietokannasta
    getByiduser: function(iduser, callback){
        return db.query("SELECT * FROM card WHERE iduser = ?", [iduser], callback);
    },

    // Hakee kortin ja siihen liittyvän tilin tiedot käyttäjän perusteella
    getCardAccountInfo: function(iduser, callback){
        return db.query("SELECT card.card_number, card.card_type, account.account_ID, account.name, account.balance FROM card JOIN account ON account.account_ID = card.account_ID WHERE card.iduser = ?",
        [iduser], callback);
    },
    // Hakee kortin ja siihen liittyvän tilin tiedot kortin numeron perusteella ei tullut käyttöön mahdollisia ideoita käyttöä varten
    getCardAccountInfoByNumber: function(card_number, callback){
        return db.query("SELECT card.card_number, card.card_type, account.idaccount, account.name, account.balance FROM card JOIN account on account.account_ID = card.account_ID WHERE card_number = ?",
        [card_number], callback);
    },
    // Hakee kortin yritysmäärän tietyn kortin numeron perusteella tämä toteutettiin qt:ssa
    getTries: function(card_number, callback){
        return db.query("SELECT tries FROM card WHERE card_number = ?", [card_number], callback);
    },
    // Päivittää kortin aktiivisuustilaa tietyn kortin numeron perusteella
    updateActiveStatus: function(active, card_number, callback){
        return db.query("UPDATE card SET active = ? WHERE card_number = ?", [active, card_number], callback);
    },
    // Liittää kortin käyttäjän tiliin
    connectCard: function(accountId, userId, card_type, pin, callback){
        // Hashataan PIN-koodi ennen kortin liittämistä tiettyyn käyttäjätiliin
        bcrypt.hash(pin, saltRounds, function(err, hash){
            return db.query("UPDATE card SET account_ID = ?, user_ID = ?, card_type = ?, pin = ?, active = 1 WHERE user_ID IS NULL AND account_ID IS NULL ORDER BY card_ID ASC LIMIT 1",
            [accountId, userId, card_type, hash], callback);
        });
    },
    // Irrottaa kortin käyttäjätilistä tietyn kortin numeron perusteella
    disconnectCard: function(card_number, callback){
        return db.query("UPDATE card SET account_ID = NULL, iduser = NULL, card_type = 0, active = 0 WHERE card_number = ?",
        [card_number], callback);
    },
    // Irrottaa kaikki kortit tietystä käyttäjätilistä
    disconnectByAccountId: function(accountId, callback){
        return db.query("UPDATE card SET account_ID = NULL, iduser = NULL, card_type = 0, active = 0 WHERE account_ID = ?",
        [accountId], callback);
    },

    // Poistaa tietyn kortin tietokannasta
    delete: function(idcard, callback){
        return db.query("DELETE FROM card WHERE idcard = ?", [idcard], callback);
    },
    // Päivittää kortin yritysmäärää tietyn kortin numeron perusteella
    updateTries: function(tries, card_number, callback){
        return db.query("UPDATE card SET tries = ? WHERE card_number = ?", [tries, card_number], callback);
    }

}

// Viedään "card" objekti moduulina käytettäväksi muissa tiedostoissa
module.exports = card;
