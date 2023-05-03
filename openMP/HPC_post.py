import csv
print("h")
def read_csv(filename):
    data = []
    with open(filename, newline='') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            data.append(row)
    return data

# Example usage
filename = './openMP/output.csv'
my_data = read_csv(filename)
# print(len(my_data[4001][41]))
import matplotlib.pyplot as plt
import numpy as np
from sklearn.decomposition import PCA

# create a list of 10 data points with 41 features each
# data = np.random.rand(10, 41)

# reduce the dimensionality of the data to 2 dimensions using PCA
pca = PCA(n_components=2)
reduced_data = pca.fit_transform(my_data)

print(len(reduced_data))

# create a scatter plot of the reduced data
fig, ax = plt.subplots()
ax.scatter(reduced_data[:,0], reduced_data[:,1])
plt.show()