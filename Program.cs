using System;
using NativeUiLib;
using System.Data;

namespace MultiCalcPro
{
    public static class Program
    {
        public static void Main()
        {
            // Create the main layout for the app
            var layout = new LinearLayout();

            // Create a single input field for entering the entire expression
            var expressionInput = new EditText { Hint = "Enter expression (e.g., 25+7/5*6, sin(30), cos(0))" };
            layout.AddView(expressionInput);

            // Create a button to calculate the expression
            var calculateButton = new Button { Text = "Calculate" };
            layout.AddView(calculateButton);

            // Create a label to display the result
            var resultLabel = new TextView { Text = "Result: " };
            layout.AddView(resultLabel);

            // Define the event handler for the button
            calculateButton.Click += delegate
            {
                // Get the input expression
                string expression = expressionInput.Text;

                try
                {
                    // Evaluate the expression using the custom method
                    var result = EvaluateExpression(expression);
                    resultLabel.Text = $"Result: {result}";
                }
                catch (Exception ex)
                {
                    // Handle invalid input
                    resultLabel.Text = "Error: Invalid expression!";
                }
            };

            // Display the entire layout
            layout.Show();
        }

        // Function to evaluate a mathematical expression from a string
        public static double EvaluateExpression(string expression)
        {
            // Replace trigonometric functions with their implementations
            if (expression.Contains("sin") || expression.Contains("cos") || expression.Contains("tan"))
            {
                expression = ParseTrigonometricFunctions(expression);
            }

            // Use DataTable to evaluate the expression safely
            var table = new DataTable();
            var result = table.Compute(expression, string.Empty);
            return Convert.ToDouble(result);
        }

        // Function to parse and replace trigonometric function calls
        private static string ParseTrigonometricFunctions(string expression)
        {
            // Handle sin(x)
            expression = ReplaceFunction(expression, "sin", Math.Sin);
            // Handle cos(x)
            expression = ReplaceFunction(expression, "cos", Math.Cos);
            // Handle tan(x)
            expression = ReplaceFunction(expression, "tan", Math.Tan);
            return expression;
        }

        // Helper function to replace function calls with calculated values
        private static string ReplaceFunction(string expression, string functionName, Func<double, double> function)
        {
            int startIndex = 0;
            while ((startIndex = expression.IndexOf(functionName, startIndex)) != -1)
            {
                // Find the opening parenthesis
                int openParenIndex = expression.IndexOf('(', startIndex);
                if (openParenIndex != -1)
                {
                    int closeParenIndex = expression.IndexOf(')', openParenIndex);
                    if (closeParenIndex != -1)
                    {
                        // Extract the argument for the function
                        string argumentString = expression.Substring(openParenIndex + 1, closeParenIndex - openParenIndex - 1);
                        if (double.TryParse(argumentString, out double argument))
                        {
                            // Calculate the result of the function
                            double result = function(argument * Math.PI / 180); // Convert degrees to radians
                            // Replace the function call with its result
                            expression = expression.Remove(startIndex, closeParenIndex - startIndex + 1)
                                                  .Insert(startIndex, result.ToString());
                        }
                        else
                        {
                            throw new Exception("Invalid trigonometric argument");
                        }
                    }
                }
                startIndex++;
            }
            return expression;
        }
    }
}