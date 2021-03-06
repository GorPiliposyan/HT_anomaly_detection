import pandas as pd
import numpy as np


def load_data(path):
    """
    -load data
    """
    data = pd.read_csv(path, header=None, sep=',')
    data.columns = ['V_all', 'sum', '1', '2', '3', '4', '5']
    # data = data.drop(['sum', 'V_all', 'V1', 'V2', 'V3', 'V4', 'V5'], axis=1)

    return data[['V_all', 'sum', '1', '2', '3', '4', '5']]


def sort_data(path):
    """
    - ????
    - ????
    """
    create_number_of_columns = 7
    data = np.genfromtxt(path, delimiter=',')
    create_number_of_rows = int(data.size/create_number_of_columns)
    print(create_number_of_rows)
    sorted_data = np.reshape(data, (create_number_of_rows, create_number_of_columns))

    return sorted_data


# file_path_1 = r'./DATA_source/POWCONS_07-10-2020_part1.txt'
# file_path_2 = r'./DATA_source/POWCONS_07-10-2020_part2.txt'
# file_path_3 = r'./DATA_source/POWCONS_07-10-2020_part3.txt'
# file_path_4 = r'./DATA_source/POWCONS_07-10-2020_part4.txt'
#
# new1 = sort_data(file_path_1)
# new2 = sort_data(file_path_2)
# new3 = sort_data(file_path_3)
# new4 = sort_data(file_path_4)
# np.savetxt('sorted_data_part_1_07-10-2020.txt', new1, fmt='%.2f', delimiter=',')
# np.savetxt('sorted_data_part_2_07-10-2020.txt', new2, fmt='%.2f', delimiter=',')
# np.savetxt('sorted_data_part_3_07-10-2020.txt', new3, fmt='%.2f', delimiter=',')
# np.savetxt('sorted_data_part_4_07-10-2020.txt', new4, fmt='%.2f', delimiter=',')












file_path_1 = r'./DATA_source/sorted_data_part_1_07-10-2020.txt'
file_path_2 = r'./DATA_source/sorted_data_part_2_07-10-2020.txt'
file_path_3 = r'./DATA_source/sorted_data_part_3_07-10-2020.txt'
file_path_4 = r'./DATA_source/sorted_data_part_4_07-10-2020.txt'
file_path_5 = r'./DATA_source/sorted_data_part_5_13-10-2020.txt'
df1 = load_data(file_path_1)
df2 = load_data(file_path_2)
df3 = load_data(file_path_3)
df4 = load_data(file_path_4)
df5 = load_data(file_path_5)

df_all = df1
df_all = df2.append(df_all)
df_all = df3.append(df_all)
df_all = df4.append(df_all)
df_all = df5.append(df_all)
np.savetxt(r'./DATA_source/ALL_DATA_MERGED.txt', df_all, fmt='%.2f', delimiter=',')
