import numpy as np 
import pandas as pd 
# from imblearn.combine import SMOTEENN
from sklearn.metrics import confusion_matrix
import os
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
#from pytorch_tabnet.tab_model import TabNetClassifier
#import torch
from sklearn.preprocessing import LabelEncoder
from sklearn.metrics import roc_auc_score
from pathlib import Path
from matplotlib import pyplot as plt
# for dirname, _, filenames in os.walk('/kaggle/input'):
#     for filename in filenames:
#         print(os.path.join(dirname,filename))

data = pd.read_csv('./dataset/KDDTrain+.txt', header = None, names=['duration','protocol_type','service','flag','src_bytes','dst_bytes','land','wrong_fragment','urgent'
            ,'hot','num_failed_logins','logged_in','num_compromised','root_shell','su_attempted','num_root'
            ,'num_file_creations','num_shells','num_access_files','num_outbound_cmds','is_host_login'
            ,'is_guest_login','count','srv_count','serror_rate','srv_serror_rate','rerror_rate','srv_rerror_rate'
            ,'same_srv_rate','diff_srv_rate','srv_diff_host_rate','dst_host_count','dst_host_srv_count'
            ,'dst_host_same_srv_rate','dst_host_diff_srv_rate','dst_host_same_src_port_rate'
            ,'dst_host_srv_diff_host_rate','dst_host_serror_rate','dst_host_srv_serror_rate','dst_host_rerror_rate'
            ,'dst_host_srv_rerror_rate','attack','outcome'])
print(len(data))
classlist = []

check1 = ("apache2","back","land","neptune","mailbomb","pod","processtable","smurf","teardrop","udpstorm","worm")
check2 = ("ipsweep","mscan","nmap","portsweep","saint","satan")
check3 = ("buffer_overflow","loadmodule","perl","ps","rootkit","sqlattack","xterm")
check4 = ("ftp_write","guess_passwd","httptunnel","imap","multihop","named","phf","sendmail","Snmpgetattack","spy","snmpguess","warezclient","warezmaster","xlock","xsnoop")


for item in data.pop('attack'):
    if item in check1:
        classlist.append("DoS")
    elif item in check2:
        classlist.append("Probe")
    elif item in check3:
        classlist.append("U2R")
    elif item in check4:
        classlist.append("R2L")
    else:
        classlist.append("Normal")

#preprocessing
#filling missing values and hot encoding
nunique = data.nunique()
types = data.dtypes
categorical_columns = []
categorical_dims =  {}
for col in data.columns:
    if types[col] == 'object':
        print(col, data[col].nunique())
        l_enc = LabelEncoder()
        print(data[col].values)
        data[col] = l_enc.fit_transform(data[col].values)
        print(data[col].values)
        categorical_columns.append(col)
        print(len(l_enc.classes_))
        categorical_dims[col] = len(l_enc.classes_)
    else:
        data.fillna(data[col].mean(), inplace=True)     #replacing the null values by mean

unused_feat = ['Set']

features = [ col for col in data.columns if col not in unused_feat ]


cat_idxs = [ i for i, f in enumerate(features) if f in categorical_columns]


cat_dims = [ categorical_dims[f] for i, f in enumerate(features) if f in categorical_columns]

# Z score Normalization
def normalize(df, cols):
    result = df.copy() 
    for feature_name in cols:
        maxi = df[feature_name].max()
        mini = df[feature_name].min()
        if(maxi>mini):
            result[feature_name] = (df[feature_name] - mini) / (maxi-mini)
    return result
data = normalize(data,data.columns)
data.drop(columns=['num_outbound_cmds'], inplace=True)  

# print(data['duration'])
N=len(data['duration'])
print(N)
print(data.columns[0])
print(len(features))
R2l_data=[]
for i in range(N):
    if(classlist[i]=='R2L'):
        r=[]
        for c in data.columns:
            r.append(data[c][i])
        R2l_data.append(r)
print(len(R2l_data[0]))
import csv



with open('./openMP/output.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(R2l_data)

