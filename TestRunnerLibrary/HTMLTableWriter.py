class HTMLTableWriter:
	def __init__ (self, fileName, title):
		self.fileName = fileName
		self.title = title
	
	def WriteHeader (self):
		self.file = open (self.fileName, 'w')
		self.file.write ('<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">\n');
		self.file.write ('<html>\n');
		self.file.write ('<head>\n');
		self.file.write ('<title>' + self.title + '</title>\n');
		self.file.write ('<style>\n');
		self.file.write ('body{font-size:12px;font-family:arial;}\n');
		self.file.write ('table{border-collapse:collapse;}\n');
		self.file.write ('td{padding:5px 10px;border:1px solid #cccccc;}\n');
		self.file.write ('tr.header{color:#000000;background:#eeeeee;font-weight:bold;}\n');
		self.file.write ('tr.row{color:#000000;background:#ffffff;}\n');
		self.file.write ('span.succeeded{color:#00aa00;}\n');
		self.file.write ('span.failed{color:#aa0000;}\n');
		self.file.write ('</style>\n');
		self.file.write ('</head>\n');
		self.file.write ('<body>\n');

	def WriteTableHeader (self, parameters):
		self.file.write ('<table>\n');	
		self.file.write ('<tr class="header">\n')
		for parameter in parameters:
			self.file.write ('<td>' + parameter + '</td>')
		self.file.write ('</tr>\n'); 		
		
	def WriteTableRow (self, parameters):
		self.file.write ('<tr class="row">\n')
		for parameter in parameters:
			self.file.write ('<td>' + parameter + '</td>')
		self.file.write ('</tr>\n'); 		

	def WriteTableFooter (self):
		self.file.write ('</table>\n');
		
	def WriteFooter (self):
		self.file.write ('</body>\n');
		self.file.write ('</html>\n');
		self.file.close ()	
