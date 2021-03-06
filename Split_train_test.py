import pandas as pd
import numpy as np


def load_data(path):
    """
    - delete column with sum (in this particular case we don't need to measure the sum)
    - delete non-unique rows
    """
    # data = pd.read_csv(r'C:\Users\Gor\Desktop\ALL.txt', header=None, sep=';')
    data = pd.read_csv(path, header=None, sep=',')
    # data.columns = ['sum', '1', '2', '3', '4', '5', 'V_all', 'V1', 'V2', 'V3', 'V4', 'V5']
    # data = data.drop(['sum', 'V_all', 'V1', 'V2', 'V3', 'V4', 'V5'], axis=1)
    data.columns = ['V_all', 'sum', '1', '2', '3', '4', '5']
    # data = data.drop(['sum', 'V_all', 'V1', 'V2', 'V3', 'V4', 'V5'], axis=1)
    # data = data[:10000]
    # data = data.drop_duplicates()

    return data[['1', '2', '3', '4', '5']]


# def moving_average_numpy(data_set, periods=3):
#     """Return numpy array of the moving averages"""
#
#     weights = np.ones(periods) / periods
#     mov_av = np.convolve(data_set, weights, mode='valid')
#
#     return np.around(mov_av, decimals=2)

def add_trojan_rows(data_set, i, num_of_trojan_rows, trojan_min, trojan_max, ht_column_choice=None):
    """ Take a random i from (0 : last - num_of_trojan_rows) and add HTs
        to rows (i : i + num_of_trojan_rows). Trojan power consumption is
        uniformly distributed in the range (trojan_min : trojan_max)"""

    trojan_power = np.random.uniform(low=trojan_min, high=trojan_max, size=(num_of_trojan_rows, 1))
    trojan_indexes = range(i, i + num_of_trojan_rows)
    data_set = np.r_[data_set]
    total_rows, total_columns = data_set.shape
    if ht_column_choice is None:
        available_columns = np.arange(total_columns)  #All columns
        ht_column = np.random.choice(available_columns)  # Choose a random column to add the HT
    else:
        ht_column = ht_column_choice
    print(ht_column)
    # ht_free_columns = np.setdiff1d(available_columns, ht_column)
    column_with_trojan = data_set[trojan_indexes, ht_column:ht_column+1] + trojan_power
    columns_without_trojan_1 = data_set[trojan_indexes, 0:ht_column]
    columns_without_trojan_2 = data_set[trojan_indexes, ht_column+1:total_columns]
    trojan_rows = np.append(columns_without_trojan_1, column_with_trojan, axis=1)
    trojan_rows = np.append(trojan_rows, columns_without_trojan_2, axis=1)
    """column_with_trojan = data_set[trojan_indexes, 0:1] + trojan_power
    columns_without_trojan = data_set[trojan_indexes, 1:5]
    trojan_rows = np.append(column_with_trojan, columns_without_trojan, axis=1)"""
    infected_data_part_1 = np.append(data_set[range(i), 0:5], trojan_rows, axis=0)
    infected_data_all = np.append(infected_data_part_1, data_set[range(i + num_of_trojan_rows, total_rows), 0:5], axis=0)
    infected_data_all = pd.DataFrame(infected_data_all, columns=['1', '2', '3', '4', '5'])

    return infected_data_all


def moving_average_panda(data_set, periods=4, drop_initial_data=True):
    """Return panda data frame of the moving averages"""

    data_set['MA_Col1'] = data_set.iloc[:, 0].rolling(window=periods).mean()
    data_set['MA_Col2'] = data_set.iloc[:, 1].rolling(window=periods).mean()
    data_set['MA_Col3'] = data_set.iloc[:, 2].rolling(window=periods).mean()
    data_set['MA_Col4'] = data_set.iloc[:, 3].rolling(window=periods).mean()
    data_set['MA_Col5'] = data_set.iloc[:, 4].rolling(window=periods).mean()
    if drop_initial_data:
        data_set.drop(['1', '2', '3', '4', '5'], axis=1, inplace=True)
    data_set.drop(range(periods), inplace=True)

    return data_set


def split_to_train_test(split_ratio, input_data):
    """ Split the input data into train data and
     test data, with the split ratio given as input"""

    data = input_data.drop_duplicates()
    data = data.sample(frac = 1)
    data = np.r_[data]
    rows, columns = data.shape
    a = int(rows*split_ratio)
    train_data = data[0: a]
    test_data = data[a: rows+1]

    return train_data, test_data

####################################################################################
# Load all the data ================================================================
# file_path = r'./POWCONS_with_voltages_2.TXT'
file_path = r'C:/Users/Gor/Desktop/New_experiments/Important_files-3/new_DATA/Split_into_train_test/DATA_source/ALL_DATA_MERGED.txt'
all_data = load_data(file_path)

averaging_level = 5

P_trojan_min = 15
P_trojan_max = 20

num_of_ht_rows = 10  # HT points per instance
total_num_of_HT_rows = 25000  # total number of HT rows to be generated
N = int(total_num_of_HT_rows/num_of_ht_rows)   # number of HT locations



buffer_zone = averaging_level + num_of_ht_rows
####################################################################################

####################################################################################
# Add Trojan rows to the data. =====================================================
all_data_numpy = np.r_[all_data]
n_all_data_rows, n_all_data_columns = all_data_numpy.shape

initial_available_index_range = np.arange(averaging_level, n_all_data_rows - num_of_ht_rows)
available_index_range = initial_available_index_range
ht_index_list = np.array([], dtype=np.int64)
# Randomly choose N indexes and HT rows
for i in range(N):
    index = np.random.choice(available_index_range)    # choose index
    print(index)
    remove_from_available_index_range = np.arange(index - buffer_zone, index + buffer_zone + 1)
    available_index_range = np.setdiff1d(available_index_range, remove_from_available_index_range)
    all_data = add_trojan_rows(data_set=all_data, i=index, num_of_trojan_rows=num_of_ht_rows,
                               trojan_min=P_trojan_min, trojan_max=P_trojan_max, ht_column_choice= None)
    # ht_index_list = np.append(ht_index_list, np.arange(index, index + buffer_zone))
    ht_index_list = np.append(ht_index_list, np.arange(index, index + num_of_ht_rows))

ht_index_list = np.sort(ht_index_list)      # This list is for later use
####################################################################################
print(ht_index_list)


####################################################################################
# Calculate the moving averages data frame =========================================
all_data = moving_average_panda(all_data, averaging_level, drop_initial_data=False)
####################################################################################


####################################################################################
# Separate trojan rows (index:index +  num_of_ht_rows + averaging_level) from clean rows (0:index) and (index +  num_of_ht_rows + averaging_level : end)
# Separate trojan rows from clean rows
# trojan_free_indexes = np.append(np.arange(averaging_level, index), np.arange(index + num_of_ht_rows + averaging_level, n_all_data_rows))
# trojan_rows = all_data.drop(trojan_free_indexes, axis=0)
# all_data_clean = all_data.drop(range(index, index + num_of_ht_rows + averaging_level), axis=0)
trojan_rows = all_data.iloc[ht_index_list, :]
all_data_clean = all_data.drop(ht_index_list, axis=0)
####################################################################################


####################################################################################
# Split the moving average data frame into train and test ==========================
all_data_clean = all_data_clean.drop_duplicates()
spl_ratio = (all_data_clean.shape[0] - N * num_of_ht_rows)/all_data_clean.shape[0]
training_data, testing_data = split_to_train_test(spl_ratio, all_data_clean)
####################################################################################

# Append Trojan rows to test data
#testing_data = np.append(testing_data, trojan_rows, axis=0)


# Save data sets in text files
np.savetxt("C:/Users/Gor/Desktop/New_experiments/Important_files-3/new_DATA/Split_into_train_test/HT3_1/4/my_training_data.txt", training_data, fmt='%.2f', delimiter=", ")
np.savetxt("C:/Users/Gor/Desktop/New_experiments/Important_files-3/new_DATA/Split_into_train_test/HT3_1/4/my_testing_data.txt", testing_data, fmt='%.2f', delimiter=", ")
np.savetxt("C:/Users/Gor/Desktop/New_experiments/Important_files-3/new_DATA/Split_into_train_test/HT3_1/4/my_trojan_data.txt", trojan_rows, fmt='%.2f', delimiter=", ")
with open( "C:/Users/Gor/Desktop/New_experiments/Important_files-3/new_DATA/Split_into_train_test/HT3_1/4/README.txt", "w") as text_file:
    print("The following parameters have been chosen to generate a {} row long HT dataset\n".format(num_of_ht_rows*N),
          "\nAveraging level = {}\nHT points = {}\nHT P_min = {}\nHT P_max = {}".format(averaging_level, num_of_ht_rows, P_trojan_min, P_trojan_max), file=text_file)


print("Done!!!")
