// Moduulien lataus
const express = require('express'); // Express-kehys
const router = express.Router(); // Express-reituri
const bcrypt = require('bcryptjs'); // Salasanan hajautusta varten
const login = require('../models/login_model'); // Kirjautumismalli
const jwt = require('jsonwebtoken'); // JWT-tunnistuksen luominen
const dotenv = require('dotenv'); // Ympäristömuuttujien lataus

// Kirjautumisen käsittely POST-pyynnöllä
router.post('/', 
  function(request, response) {
    console.log('login test');
    // Tarkistaa, onko korttinumero ja PIN-koodi saatu pyynnön mukana
    if(request.body.card_number && request.body.pin){
      const card = request.body.card_number;
      const pass = request.body.pin;
      
      // Tarkistaa PIN-koodin tietokannasta
      login.checkpin(card, function(dbError, dbResult) {
        if(dbError){
          response.json(dbError);
        }
        else{
          if (dbResult.length > 0) {
            // Vertaa salattua PIN-koodia annettuun PIN-koodiin
            bcrypt.compare(pass, dbResult[0].pin, function(err, compareResult) {
              if(compareResult) {
                console.log("success");
                // Jos vertailu onnistuu, luo JWT-tokeni ja lähettää se vastauksena
                const token = generateAccessToken({ card_number: card });
                response.send(token);
              }
              else {
                console.log("wrong pin");
                response.send(false);
              }			
            });
          }
          else{
            console.log("card_number does not exists");
            response.send(false);
          }
        }
      });
    }
    else{
      console.log("card_number or pin missing");
      response.send(false);
    }
  }
);

// JWT-tokenin luominen
function generateAccessToken(card_number) {
  dotenv.config();
  // Luo JWT-tokeni annetulla korttinumerolla ja asettaa sen vanhenemaan 1800 sekunnin kuluttua
  return jwt.sign(card_number, process.env.MY_TOKEN, { expiresIn: '1800s' });
}

module.exports = router; // Vienti
