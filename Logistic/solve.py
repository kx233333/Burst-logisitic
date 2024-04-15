import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import f1_score

last_dict = {}
pre = []
data = pd.read_csv('../Logistic/data/data.csv')
# print(data)
X = data.iloc[:, :-1]
y = data.iloc[:, -1]
# print(X, y)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

SVCmodel = SVC(kernel="rbf")

SVCmodel.fit(X_train, y_train)

y_pred = SVCmodel.predict(X_test)
print(y_pred[y_test == 1])
f1 = f1_score(y_test, y_pred)

# logreg = LogisticRegression()
# logreg.fit(X_train, y_train)
print("SVC f1score:", f1)

# y_pred = logreg.predict(X_test)
# print(y_pred[y_test == 1])
# f1 = f1_score(y_test, y_pred)

# print("logistic f1score:", f1)
