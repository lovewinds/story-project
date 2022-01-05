# from pyemb_test import hello
# from scriptHostModule import scriptHost


class hello:
    def __init__(self, c: str) -> None:
        self.country = c

    def greet(self) -> None:
        return f'Hello from {self.country}'

class ScriptEntryPoint:
    def __init__(self) -> None:
        pass

    def check(self) -> str:
        print(hello)
        print(hello.__dict__)
        # print(scriptHost)
        # print(scriptHost.__dict__)
        h = hello('python')
        print(h)
        return h.greet()
        # return scriptHost.getBasePath

if __name__ == "__main__":
    s = ScriptEntryPoint()
    m = s.check()
    print(f'Message : {m}')
