import { readAll } from "https://deno.land/std@0.212.0/io/read_all.ts";

// FIXME: Loaded from arg.
const testPort = 27015;

export type TestObject = {
	name: string;
	test: () => Promise<void>;
};

export type TestModule = TestObject[];

export async function makeRequest(request: Request): Promise<Response>;
export async function makeRequest(request: string[]): Promise<Response>;

export async function makeRequest(
	request: Request | string[]
): Promise<Response> {
	let requestString: string;
	if (request instanceof Request) {
		requestString = request.toString();
	} else {
		requestString = request.join("\r\n") + "\r\n\r\n";
	}

	const conn = await Deno.connect({
		port: testPort,
		hostname: "127.0.0.1",
	});

	const encoder = new TextEncoder();

	await conn.write(encoder.encode(requestString));

	const response = await readAll(conn);

	conn.close();

	return new Response(response, {
		// FIXME: This should probably be removed.
		headers: {
			"Content-Type": "text/plain",
		},
	});
}
