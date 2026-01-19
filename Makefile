clean:
	rm -rf .idea

# GIT HELPER

MESSAGE = .

push: clean add commit
	git push

add:
	git add README.md x11.h xKeys.h xMath.h xCamera.h xModel.h

commit:
	git commit -a -m "$(MESSAGE)"