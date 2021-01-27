from lab0_utilities import *
	

# NOTE - Rotations and insertions and balancing are available from avl_trees_tester
# build_trees_from_file is the main idea behind building the tree, getting this wrong implies
# most of the testable functions are also wrong. Balancing is secondary, getting the nodes into 
# the tree in the right format is more important.

class Languages:
	def __init__(self):
		self.data_by_year = {} #{int: BalancingTree}


	def build_trees_from_file(self, file_object):
		# Update the self.data_by_year attribute and return (return not checked directly)
		data = file_object.readlines()
		for line in data[1:]:
			year, language, count = line.split(',')
			year, count = int(year), int(count) 

			language_stat = LanguageStat(language, year, count)
			node = Node(language_stat)

			#print('Inserting.....', node._val, ' in year ', year)
			if year not in self.data_by_year:
				self.data_by_year[year] = BalancingTree(node)
			else:
				self.data_by_year[year].balanced_insert(node, self.data_by_year[year].root)
		return self.data_by_year	


	def query_by_name(self, language_name):
		# Return a new dictionary of the form {year: count} for the language name
		result = {}
		for year in self.data_by_year.keys():
			count = self.findName(self.data_by_year[year].root, language_name)
			if count:
				result[year] = count
		
		return result
				

	def query_by_count(self, threshold = 0):
		# Return a new dictionary of the form {year: List(str)}
		result = {}
		for year in self.data_by_year.keys():
			names = []
			self.findCounts(self.data_by_year[year].root, threshold, names)
			if names:
				result[year] = names
		return result


	def findName(self, node, language_name):
		if node is None:
			# We hit None only when data does not exist in the tree
			return False
		if language_name in node._val:
			return node.val.count
		elif language_name < node._val:
			return self.findName(node.left, language_name)
		else:
			return self.findName(node.right, language_name)


	def findCounts(self, node, threshold, names):
		if node is None:
			return
		if node.val.count > threshold:
			names.append(node.val.name)
		self.findCounts(node.left, threshold, names)
		self.findCounts(node.right, threshold, names)
		return


class BalancingTree:
	def __init__(self, root_node):
		self.root = root_node
	
	
	# Function used in balanced_insert and in grading rotations directly
	def insert(self, node, curr = None):
		curr = curr if curr else self.root
		# Recurse to right spot
		if node._val < curr._val:
			if curr.left is not None:
				self.insert(node, curr.left)
			else:
				node.parent = curr
				curr.left = node
		else:
			if curr.right is not None:
				self.insert(node, curr.right)
			else:
				node.parent = curr
				curr.right = node
		return
		

	def balanced_insert(self, node, curr = None):
		curr = curr if curr else self.root
		self.insert(node, curr)
		self.balance_tree(node)
		return


	# Check balance manually with heights for grading purposes
	# Commented code for checking balance with bf's
	def find_height(self, root):
		if not root:
			return 0
		return 1 + max(self.find_height(root.left), self.find_height(root.right))
	
	def is_balanced(self):
		root = self.root
		def isBalancedHelper(root):
			if root is None:
				return True
			h1 = self.find_height(root.left)
			h2 = self.find_height(root.right)
			if abs(h1-h2) < 2 and isBalancedHelper(root.left) and isBalancedHelper(root.right):
				return True
			return False
		
		return isBalancedHelper(root)
		
		'''
		# bf implementation for quick checks
		# Helper
		def isBalancedHelper(root):
			if not root:
				return True
			if abs(root.bf) < 2:
				return isBalancedHelper(root.left) and isBalancedHelper(root.right)
			return False
		
		return isBalancedHelper(root)
		'''

	

	# NOTE - Balancing done with heights and not bf with a default height of 1
	# avl_trees_tester.py exercises gives the balance implementation using bf's
	# Needs to be modified if using default height of 0
	# maximum abs(bf) = 2 (usually never happens), modify for 1

	def balance_tree(self, node):
		# Bubbles up from inserted node -> check avl_trees_tester for implementation that starts from root
		while node is not None:
			self.update_height(node)
			if self.height(node.left) >= 2 + self.height(node.right):
				if self.height(node.left.left) >= self.height(node.left.right):
					self.right_rotate(node)
				else:
					self.left_rotate(node.left)
					self.right_rotate(node)
			elif self.height(node.right) >= 2 + self.height(node.left):
				if self.height(node.right.right) >= self.height(node.right.left):
					self.left_rotate(node)
				else:
					self.right_rotate(node.right)
					self.left_rotate(node)
			node = node.parent



	# Change left and right rotate to avl_trees_tester for for default height 0 and balancing with bf
	def left_rotate(self, x):
		y = x.right
		y.parent = x.parent
		if y.parent is None:
			self.root = y
		else:
			if y.parent.left is x:
				y.parent.left = y
			elif y.parent.right is x:
				y.parent.right = y
		x.right = y.left
		if x.right is not None:
			x.right.parent = x
		y.left = x
		x.parent = y
		self.update_height(x)
		self.update_height(y)
 

	def right_rotate(self, x):
		y = x.left
		y.parent = x.parent
		if y.parent is None:
			self.root = y
		else:
			if y.parent.left is x:
				y.parent.left = y
			elif y.parent.right is x:
				y.parent.right = y
		x.left = y.right
		if x.left is not None:
			x.left.parent = x
		y.right = x
		x.parent = y
		self.update_height(x)
		self.update_height(y)


### Extra helper functions from here


	def update_height(self, node):
		node.height = 1 + max(self.height(node.left), self.height(node.right))


	def height(self, node):
		return node.height if node else -1


	def inOrder(self, node):
		if node is None:
			return	
		self.inOrder(node.left)
		print(node._val)
		self.inOrder(node.right)
	

	def preOrder(self, node):
		if node is None:
			return
		print(node._val)
		self.inOrder(node.left)
		self.inOrder(node.right)


if __name__ == "__main__":
	with open('data/us_languages.csv') as f:
		languages = Languages()
		data_by_year = languages.build_trees_from_file(f)

	root = data_by_year[2000].root

	q = []
	q.append(root)
	while q:
		nodes = len(q)
		while nodes > 0:
			node = q.pop(0)
			print(node._val, end=' ')
			if node.left is not None:
				q.append(node.left)
			if node.right is not None:
				q.append(node.right)
			nodes -= 1
		print(' ')
	print()

	print(data_by_year[2000].is_balanced())
	print()

	print('\nQuerying for French: ', languages.query_by_name('French'))
	print('Querying for English: ', languages.query_by_name('English'))
	print('Querying for random lang: ', languages.query_by_name('asdfasdf'))

	print('\nQuerying by count = 200000: ', languages.query_by_count(200000))
	print('Querying by count = 500000: ', languages.query_by_count(500000))
	print('Querying by count = 300000: ', languages.query_by_count(300000))

	#print('Height difference for 1931', data_by_year[1931].get_height_difference())
	#print('Height difference for 1941', data_by_year[1941].get_height_difference())
	#print('Height difference for 1971', data_by_year[1971].get_height_difference())`