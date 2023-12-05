var sheetID = '';  // Replace with your actual spreadsheet ID

function doGet(e) {
  var temperature = e.parameter.temperature;
  var humidity = e.parameter.humidity;
  var sheet = SpreadsheetApp.openById(sheetID).getActiveSheet();
  
  var dateTime = new Date();
  var currentDate = Utilities.formatDate(dateTime, "Europe/London", "dd/MM/yyyy");  // Replace Timezone ID and format
  var currentTime = Utilities.formatDate(dateTime, "Europe/London", "HH:mm:ss");  // Replace Timezone ID and format

  sheet.appendRow([currentDate, currentTime, temperature, humidity]);

  return ContentService.createTextOutput('Data logged successfully');
}