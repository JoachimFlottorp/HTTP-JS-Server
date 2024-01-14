import { makeRequest, TestModule } from "./prelude.mts";
import { assert } from "https://deno.land/std@0.212.0/assert/assert.ts";

export default [
	{
		name: "Basic",
		test: async () => {
			const response = await makeRequest([
				"GET / HTTP/1.1",
				"Host: localhost:8080",
				"User-Agent: curl/7.54.0",
				"Accept: */*",
				"X-Empty: ",
			]);

			assert(response.status === 200, "Status code should be 200");
			assert(
				response.headers.get("Content-Type") === "text/plain",
				"Content-Type should be text/plain"
			);
		},
	},
] as TestModule;
