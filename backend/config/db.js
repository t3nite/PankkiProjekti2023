// MySQL-tietokantamoduulin ja ympäristömuuttujien lataus
var mysql = require("mysql2");
require("dotenv").config();

// MySQL-tietokantayhteyden luominen
const conn = mysql.createPool({
  host: process.env.DB_HOST, // Tietokantapalvelimen osoite
  user: process.env.DB_USER, // Käyttäjänimi
  password: process.env.DB_PASS, // Salasana
  database: process.env.DB_DB, // Tietokannan nimi
  multipleStatements: "true" // Sallitaan useiden SQL-lauseiden suorittaminen yhdellä kyselyllä
})

// Viedään yhteys moduulille, joka käyttää sitä
module.exports = conn;
