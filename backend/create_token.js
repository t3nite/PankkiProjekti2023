// Moduulien lataus
var crypto = require('crypto'); // Tarvitaan satunnaisen tavumerkkijonon luomiseen
var co = require('co'); // Generaattorifunktioiden käyttöön

// Funktio, joka palauttaa toiminnon, joka luo satunnaisen tavumerkkijonon
function spawnTokenBuf() {
    return function(callback) {
        crypto.randomBytes(64, callback); // Luodaan 64 tavua pitkä satunnainen tavumerkkijono
    };
}

// Generaattorifunktio, joka käyttää co-moduulia
co(function* () {
    // Tulostetaan base64-muotoinen tavumerkkijono konsoliin
    console.log((yield spawnTokenBuf()).toString('base64'));
});