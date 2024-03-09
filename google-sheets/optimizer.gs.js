/**
 * Attempts to maximize the value in the target cell by adjusting the values in a given range using a binary search approach.
 * @param {string} targetCellAddress The address of the cell to maximize (e.g., "Sheet1!A1").
 * @param {string} variableRangeAddress The address of the range to vary (e.g., "Sheet1!B1:B10").
 * @param {number} minVal The minimum value a variable can take.
 * @param {number} maxVal The maximum value a variable can take.
 * @param {number} tolerance The tolerance within which the optimal value is acceptable.
 */
function maximizeTargetCellValueBinarySearch(targetCellAddress, variableRangeAddress, minVal, maxVal, tolerance) {
  const ss = SpreadsheetApp.getActiveSpreadsheet();
  const targetCell = ss.getRange(targetCellAddress);
  const variableRange = ss.getRange(variableRangeAddress);
  let variables = variableRange.getValues();
  
  for (let i = 0; i < variables.length; i++) {
    let low = minVal;
    let high = maxVal;
    while (low <= high) {
      let mid = low + (high - low) / 2;
      // Set the current variable to mid-point value
      variables[i][0] = mid;
      variableRange.setValues(variables);
      let currentValue = targetCell.getValue();
      
      // Increase the mid-point to see if the target value increases
      variables[i][0] = mid + tolerance;
      variableRange.setValues(variables);
      let newValue = targetCell.getValue();
      
      if (newValue > currentValue) {
        low = mid + tolerance;
      } else {
        high = mid - tolerance;
      }
      
      // Check if the search space is smaller than tolerance
      if (Math.abs(high - low) < tolerance) {
        break; // Stop if the difference is within the tolerance
      }
    }
    // Set the variable to the optimal value within the tolerance
    variables[i][0] = low;
    variableRange.setValues(variables);
  }
  
  Logger.log("Maximization complete. Check the spreadsheet for results.");
}


function runOptimizationBinarySearch() {
  const targetCellAddress = 'Sheet1!B2';
  const variableRangeAddress = 'Sheet1!A2:A3';
  const minVal = 0; // Minimum possible value for variables
  const maxVal = 100; // Maximum possible value for variables
  const tolerance = 1; // Acceptable tolerance level

  maximizeTargetCellValueBinarySearch(targetCellAddress, variableRangeAddress, minVal, maxVal, tolerance);
}

