#! /usr/bin/env -S deno run --allow-env

import { TestModule } from "./prelude.mts";

// NOTE: Preferred way would to load modules at runtime.
//       But that got really cancerous really fast.
//       "error: Uncaught Error: The filename, directory name, or volume label syntax is incorrect"
const testFiles = ["basic.mts"] as const;

for (const testFile of testFiles) {
	const module: { default: TestModule } = await import(`./${testFile}`);

	for (const [key, value] of Object.entries(module.default)) {
		if (typeof value === "function") {
			Deno.test(key, value);
		} else {
			Deno.test(key, value.test);
		}
	}
}
