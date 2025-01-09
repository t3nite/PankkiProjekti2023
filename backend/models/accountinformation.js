const db = require("../config/db.js");

const accountinformation = {
    get: function(callback){
        return db.query("SELECT * FROM accountinformation ORDER BY log_ID DESC", callback);
    },
    getMaxaccountinformationNum: function(card_number, callback){
        return db.query("SELECT COUNT(accountinformation_ID) AS maxCount FROM accountinformation WHERE account_ID = (SELECT card.account_ID FROM card WHERE card_number = ?)", [card_number], callback);
    },
    getByCardNumberFixed: function(card_number, callback){
        return db.query("SELECT * FROM accountinformation WHERE account_ID = (SELECT card.account_ID FROM card WHERE card_number = ?) ORDER BY log_ID DESC LIMIT 10", [card_number], callback);
    },
    getByCardNumber: function(card_number, x, callback){
        return db.query("SELECT * FROM accountinformation WHERE account_ID = (SELECT card.account_ID FROM card WHERE card_number = ?) ORDER BY log_ID DESC LIMIT ?, 10", [card_number, x], callback);
    },
    add: function(req, event, callback){
        return db.query("INSERT INTO accountinformation (date, event, amount, account_ID) VALUES(DATE_FORMAT(current_timestamp(), '%d.%c.%Y %H:%i'), ?,?, (SELECT card.account_ID FROM card WHERE card_number = ?))",
        [event, req.body.amount, req.body.card_number], callback);
    },
    delete: function(req, callback){
        return db.query("DELETE FROM accountinformation WHERE account_ID = (SELECT card.account_ID FROM card WHERE card_number = ?) AND accountinformation_ID = ?",
        [req.body.card_number, req.body.log_ID], callback);
    }

}

module.exports = accountinformation;