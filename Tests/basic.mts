import { makeRequest, TestModule } from "./prelude.mts";
import { assert } from "https://deno.land/std@0.212.0/assert/assert.ts";

export default {
	Basic: async () => {
		const response = await makeRequest([
			"GET / HTTP/1.1",
			"Host: localhost:8080",
			"User-Agent: curl/7.54.0",
			"Accept: */*",
			"X-Empty: ",
		]);

		assert(response.status === 200, "Status code should be 200");
	},
	Headers: async () => {
		// A lot of weird shaped and many headers
		const response = await makeRequest([
			"GET / HTTP/1.1",
			"Host: localhost:8080",
			"User-Agent: curl/7.54.0",
			"Accept: */*",
			"X-Empty: ",
			"Foo: bar",
			"Xd: \r\r\r\r\n",
		]);

		assert(response.status === 200, "Status code should be 200");
	},
} as TestModule;
