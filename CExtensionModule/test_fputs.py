# test with pytest
# pytest -q

import fputs

def test_copy_data():
	content_to_copy = "Real Python!"
	success = fputs.fputs(content_to_copy, "test_write.txt")

	with open("test_write.txt", "r") as f:
		content_copied = f.read()

	assert content_copied == content_to_copy