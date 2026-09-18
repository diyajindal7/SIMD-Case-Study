import pandas as pd
import pickle

from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score, confusion_matrix, classification_report

# Load the dataset
data = pd.read_csv("simd_ai_dataset_60.csv")

print("Dataset loaded successfully!")
print("Number of rows:", len(data))

# Input features
X = data[
    [
        "data_size",
        "operation_complexity",
        "memory_footprint",
        "available_cores"
    ]
]

# Target
y = data["simd_significant"]

print("\nClass distribution:")
print(y.value_counts())

# Split dataset into training and testing data
X_train, X_test, y_train, y_test = train_test_split(
    X,
    y,
    test_size=0.20,
    random_state=42,
    stratify=y
)

print("\nTraining samples:", len(X_train))
print("Testing samples :", len(X_test))

# Create Decision Tree model
model = DecisionTreeClassifier(
    max_depth=3,
    random_state=42
)

# Train the AI model
model.fit(X_train, y_train)

# Predict test data
y_pred = model.predict(X_test)

# Calculate accuracy
accuracy = accuracy_score(y_test, y_pred)

print("\n========================================")
print("          AI MODEL EVALUATION")
print("========================================")

print("\nActual values    :", y_test.values)
print("Predicted values :", y_pred)

print("\nAccuracy:", accuracy)

print("\nConfusion Matrix:")
print(confusion_matrix(y_test, y_pred))

print("\nClassification Report:")
print(classification_report(
    y_test,
    y_pred,
    zero_division=0
))

# Save trained model
with open("simd_model.pkl", "wb") as file:
    pickle.dump(model, file)

print("\nAI model saved successfully!")
print("File: simd_model.pkl")