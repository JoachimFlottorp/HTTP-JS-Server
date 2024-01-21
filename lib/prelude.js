Duktape.errCreate = function (err) {
    try {
        if (typeof err === 'object' &&
            typeof err.message !== 'undefined' &&
            typeof err.lineNumber !== 'undefined') {
            err.message = String(err.message) + ' (line ' + String(err.lineNumber) + ')';
        }
    } catch (e) {
        // ignore
    }

    return err;
};

globalThis.console = new Proxy(new ScriptLogging(), {
    get: function (target, prop) {
        if (prop in target) {
            return target[prop];
        }
        return target.log;
    }
});

Object.freeze(globalThis.console);

globalThis.__functionStore = [];
globalThis.root = new Router("/");

root
    .get("/", function (req, res) {
        res.setText("Hello World!");
    })
    .get("/hello/:name", function (req, res) {
        const name = req.params.name;

        // res.sendText(`Hello ${name}!`);
    })
    .get("/hello/:age{number}", function (req, res) {
        const age = req.params.age;

        // res.sendText(`You are ${age} years old.`);
    });

const fooRouter = new Router("/foo")

fooRouter
    .get("/", function (req, res) {
        res.setText("Hello Foo!");
    })
    .get("/bar", function (req, res) {
        console.log("xd");

        res.setText("Hello Bar!");
    });

console.log(__functionStore.length + " routes registered.");
