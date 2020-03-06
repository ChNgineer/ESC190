from lab0_utilities import *

class Languages:
	def __init__(self):
		self.data_by_year = {}

	def build_trees_from_file(self, file_object):
		f = file_object
		file_string = f.read().split('\n')
		file_vector = []
		language_list = []

		for i in range(1, len(file_string)-1):
			file_vector.append(file_string[i].split(','))
			language = LanguageStat(file_vector[i - 1][1], file_vector[i - 1][0], file_vector[i - 1][2])
			language_list.append(language)

		existing_year = []
		checked_languages = []
		language_by_year = []

		for i in range(len(language_list)):
			if language_list[i].year not in existing_year:
				existing_year.append(language_list[i].year)
				checked_languages = [language_list[i]]
				language_by_year.append(checked_languages)
			else:
				checked_languages.append(language_list[i])

		for i in range(len(language_by_year)):
			root = Node(language_by_year[i][0])
			avl = BalancingTree(root)
			for j in range(len(language_by_year[i])):
				#print(language_by_year[i][j].name + ' ' + language_by_year[i][j].year)
				node = Node(language_by_year[i][j])
				node.height = avl.update_height(node)
				node.bf = avl.find_balance_factor(node)
				avl.balanced_insert(node)
				self.data_by_year.update({int(language_by_year[i][j].year): avl})
		return self.data_by_year


	def query_by_name(self, language_name):
		# implement
		name_dict = {}
		tree_list = list(self.data_by_year.values())
		print(tree_list)
		for i in range(len(tree_list)):
			if tree_list[i].search_by_name(language_name).name == language_name:
				name_dict.update({int(tree_list[i].search_by_name(language_name).year): int(tree_list[i].search_by_name(language_name).count)})
		return(name_dict)


	def query_by_count(self, threshold = 0):
		# implement
		count_dict = {}
		tree_list = list(self.data_by_year.values())
		#print(tree_list)
		for i in range(len(tree_list)):
			count_dict.update({tree_list[i].root.val.year: tree_list[i].search_by_count(threshold)})
			#print(tree_list[i].root.val.year)
		return count_dict


class BalancingTree:
	def __init__(self, root_node):
		self.root = root_node
	
	def balanced_insert(self, node, curr = None):
		curr = curr if curr else self.root
		self.insert(node, curr)
		self.balance_tree(node)


	def insert(self, node, curr = None):
		if curr._val == node._val:
			return
		curr = curr if curr else self.root
		# insert at correct location in BST
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


	def balance_tree(self, node):
		# implement
		if (node.right == None) and (node.left == None):
			return self
		if node.bf > 0:
			if node.right and node.right.bf >= 0:
				self.left_rotate(node)
			else:
				self.right_rotate(node.right)
				self.left_rotate(node)
		else:
			if node.left and node.left.bf <= 0:
				self.right_rotate(node)
			else:
				self.left_rotate(node.left)
				self.right_rotate(node)
		return self


	def update_height(self, node):
		node.height = 1 + max(self.height(node.left), self.height(node.right))


	def height(self, node):
		return node.height if node else -1


	def left_rotate(self, z):
		y = z.right
		y.parent = z.parent
		if y.parent is None:
			self.root = y
		else:
			if y.parent.left is z:
				y.parent.left = y
			elif y.parent.right is z:
				y.parent.right = y
		z.right = y.left
		if z.right is not None:
			z.right.parent = z
		y.left = z
		z.parent = y
		self.update_height(z)
		self.update_height(y)


	def right_rotate(self, z):
		# implement
		# use left_rotate as a reference
		y = z.left
		y.parent = z.parent
		if y.parent is None:
			self.root = y
		else:
			if y.parent.right is z:
				y.parent.right = y
			elif y.parent.left is z:
				y.parent.left = y
		z.left = y.right
		if z.left is not None:
			z.left.parent = z
		y.right = z
		z.parent = y
		self.update_height(z)
		self.update_height(y)


	def find_balance_factor(self, node):
		# implement
		if node == type(Node):
			return node.left.height - node.right.height


	def is_balanced(self):
		#implement
		if self.find_balance_factor(self.root) in [-1, 0, 1]:
			return True
		else:
			return False

	def search_by_name(self, val, curr_node=None):
		if curr_node is None:
			curr_node = self.root
		if curr_node.val.name == val:
			return curr_node.val
		elif curr_node.val.name > val:
			if curr_node.left:
				return self.search_by_name(val, curr_node.left)
			else:
				return False
		else:
			if curr_node.right:
				return self.search_by_name(val, curr_node.right)
			else:
				return False


	def search_by_count(self, val, curr_node=None):
		global val_list
		if curr_node is None:
			curr_node = self.root
		#print(curr_node.bf)
		if int(curr_node.val.count) > val:
			val_list.append(curr_node.val.name)
			print(curr_node.val.name + ' ' + curr_node.val.year)
		if curr_node.left:
			self.search_by_count(val, curr_node.left)
		if curr_node.right:
			self.search_by_count(val, curr_node.right)

		return list(dict.fromkeys(val_list))
val_list = []