#! /usr/bin/env -S deno run --allow-env

// NOTE: Preferred way would to load modules at runtime.
//       But that got really cancerous really fast.
//       "error: Uncaught Error: The filename, directory name, or volume label syntax is incorrect"
const testFiles = ["basic.mts"] as const;

for (const testFile of testFiles) {
	const module = await import(`./${testFile}`);

	for (const test of module.default) {
		Deno.test(test.name, test.test);
	}
}
