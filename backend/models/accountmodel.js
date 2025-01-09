// Tarvittavien moduulien lataus
const db = require("../config/db.js"); // Tietokantayhteyden konfiguraatiomoduuli
const bcrypt = require("bcrypt"); // Salasanan hashausmoduuli

// Tili-olio, joka sisältää erilaisia toimintoja tilien käsittelyyn
const account = {
    // Hakee kaikki tilit tietokannasta
    get: function(callback){
        return db.query("SELECT * FROM account", callback);
    },
    // Hakee tilin saldon tietyn tilin ID:n perusteella
    getaccountbalance: function(id, callback){
        return db.query("SELECT account_balance FROM account WHERE idaccount = ? ",[id], callback);
    },
    // Hakee tilin luottorajan tietyn tilin ID:n perusteella
    getcreditlimit: function(id, callback){
        return db.query("SELECT credit_limit FROM account WHERE idaccount = ? ",[id], callback);
    },
    // Päivittää tilin saldoa
    updateBalance: function(account_balance, callback){
        return db.query("UPDATE account SET account_balance = CASE WHEN (account_balance - ?) >= 0 THEN account_balance - ? ELSE account_balance END WHERE idaccount = 1;",
        [account_balance,account_balance], callback);
    },
    // Päivittää tilin luottorajaa
    updatecredit: function(credit_limit,callback){
        return db.query("UPDATE account SET credit_limit = CASE WHEN (credit_limit - ?) >= -100 THEN credit_limit - ? ELSE credit_limit END WHERE idaccount = 4;",
        [credit_limit,credit_limit], callback);
    },
    // Hakee käyttäjän omistamat tilit
    getByiduser: function(req, callback){
        return db.query("SELECT account.* FROM account JOIN user_account ON user_account.idaccount = account.idaccount JOIN user ON user.iduser = user_account.iduser WHERE user.iduser = ?",
        [req.iduser], callback);
    },
    // Hakee tilin kortinumeron perusteella
    getByCardNumber: function(card_number, callback){
        return db.query("SELECT account.*, card.card_type FROM account JOIN card ON card.idaccount = account.idaccount WHERE card_number = ?",
        [card_number], callback);
    },
    // Hakee tarkasti yhteen kuuluvat käyttäjät
    getExact: function(iduser, idaccount, callback){
        return db.query("SELECT * FROM user_account WHERE iduser = ? AND idaccount = ?",
        [iduser, idaccount], callback);
    },
    // Hakee yhteen liitetyt käyttäjät
    getConnectedUsers: function(idaccount, iduser, callback){
        return db.query("SELECT account.idaccount, user.email, user_account.user_iduser FROM account JOIN user_account ON user_account.idaccount = account.idaccount JOIN user ON user.iduser = user_account.iduser WHERE account.idaccount = ? AND user.iduser != ?",
        [idaccount, iduser], callback);
    },
    // Hakee kortit tilin ID:n perusteella
    getCardsById: function(idaccount, callback){
        return db.query("SELECT * FROM account JOIN card on card.idaccount = account.idaccount WHERE account.idaccount = ?",
        [idaccount], callback);
    },
    // Hakee tilin omistajan käyttäjän ID:n ja tilin ID:n perusteella
    getOwnerById: function(iduser, idaccount, callback){
        return db.query("SELECT account.idaccount, account.account_balance, account.owner FROM account JOIN user_account ON user_account.idaccount = account.idaccount JOIN user ON user.iduser = user_account.iduser WHERE user.iduser = ? AND account.idaccount = ?",
        [iduser, idaccount], callback);
    },
    // Lisää uuden tilin tietokantaan
    add: function(newaccount, callback){
        console.log("model");
        return db.query("INSERT INTO account(name, account_strated, owner, credit_limit, account_balance) VALUES(?, ?, ?, ?, ?)",
        [newaccount.name, newaccount.account_strated,newaccount.owner,newaccount.credit_limit,newaccount.account_balance], callback);
    },
    // Lisää käyttäjän tilille
    addUserToAccount: function(iduser, idaccount, callback){
        return db.query("INSERT INTO user_account(user_iduser, account_idaccount) VALUES(?, ?)", [iduser, idaccount], callback);
    },
    // Poistaa käyttäjän tililtä
    deleteUserFromAccount: function(idaccount, iduser, callback){
        return db.query("DELETE FROM user_account WHERE idaccount = ? AND iduser = ?", [idaccount, iduser], callback);
    },
    // Poistaa tilin
    delete: function(idaccount, callback){
        return db.query("DELETE FROM account WHERE idaccount = ?", [idaccount], callback);
    }
}

// Viedään account-olio moduulina käytettäväksi muissa tiedostoissa
module.exports = account;
