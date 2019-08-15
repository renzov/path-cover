import csv
from os import listdir
from os.path import isdir, join, exists, expanduser

OUTPUT_DIR = expanduser("~") + "/path-cover/Nova-Formulacao/MINPC/inteira/output"


def get_sol( filename ):
	stream = open( filename, 'r' )
	lines = stream.readlines()
	stream.close()
	return int( lines[0] )


# Take the names of every directory and sort then by name
dirs = []
for d in listdir(OUTPUT_DIR):
	dirname = join(OUTPUT_DIR, d)	
	if isdir(dirname):
		dirs.append(d)
		
dirs.sort()
# Get the filenames
filenames = []
for f in listdir( join(OUTPUT_DIR, dirs[0]) ):
	filenames.append(f)

filenames.sort()

headers = ['Nome']
for f in filenames:
	headers.append(f)

# Create the csv file
csvfile = open('sol-MINPC.csv', 'w')
writer = csv.writer( csvfile )
writer.writerow(headers)

# Now iterate through every dir and check each file
for d in dirs:
	dirname = join( OUTPUT_DIR, d )
	row = [ d ]
	for f in filenames:
		filename = join( dirname, f )
		if exists(filename):
			# Get the execution time from the path	
			row.append( get_sol(filename) )
		else:
			# Print blanck
			row.append('')
	writer.writerow(row)

csvfile.close()
