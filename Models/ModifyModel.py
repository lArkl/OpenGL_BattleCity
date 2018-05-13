#fileName = 'Caral_piramide_cesar.obj'
#fileName = 'Castillo/Castillo.obj'
fileName = 'Tank/BaseTank.obj'

import pandas as pd
data = pd.read_csv(fileName,names=['Text'])
#data.columns = ['Text']
data['Text'].replace({'//': '/0/'}, inplace=True, regex=True)

data.to_csv('Tank/BaseTank.obj',index=False)

