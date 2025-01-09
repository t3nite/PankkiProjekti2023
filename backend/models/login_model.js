// Moduulin lataus: tietokantayhteysmoduuli
const db = require("../config/db.js");

// login-olio, joka sisältää tietokantakyselyn PIN-koodin tarkistamiseksi korttinumeron perusteella
const login = {
  // Metodi, joka tarkistaa PIN-koodin tietokannasta korttinumeron perusteella
  checkpin: function(card_number, callback) {
    // Suorittaa tietokantakyselyn, joka hakee PIN-koodin korttinumeron perusteella
    return db.query('SELECT pin FROM card WHERE card_number = ?',[card_number], callback); 
  }
};

module.exports = login; // Vienti, jotta login-olio voidaan käyttää muualla sovelluksessa
