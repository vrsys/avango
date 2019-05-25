import json

# the print displays:

with open('test.json', 'r') as f:
    study_dict = json.load(f)

    print(study_dict['locations'])
    locations = study_dict['locations']
    print(len(study_dict['locations']))
    n = len(study_dict['locations'])
    
    for i in range(n):
    	print(' \n')
    	print(locations[i])