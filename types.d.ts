declare function require(name: string): any;

type HTTPParams = Record<string, any>;

interface HTTPRequest {
    params: HTTPParams;
}

interface HTTPResponse {

}

interface HTTPHandler {
    (request: HTTPRequest, response: HTTPResponse): void;
}

declare class Router {
    constructor(prefixPath: string);

    get(path: string, handler: HTTPHandler): Router;

    post(path: string, handler: HTTPHandler): Router;

    put(path: string, handler: HTTPHandler): Router;

    delete(path: string, handler: HTTPHandler): Router;
}

declare const root: Router;
