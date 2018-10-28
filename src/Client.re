/* Create an InMemoryCache */
let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(
    ~uri="https://k4d5m48aj8.execute-api.eu-west-1.amazonaws.com/dev/graphql",
    (),
  );

let authLink =
  ApolloLinks.createContextLink(() =>
    {
      "headers": {
        "X-GitHub-Token": Storage.getConfig("github"),
        "X-Trello-Key": Storage.getConfig("trello_key"),
        "X-Trello-Token": Storage.getConfig("trello_token"),
        "X-Trello-Username": Storage.getConfig("trello_username"),
      },
    }
  );

let instance =
  ReasonApollo.createApolloClient(
    ~link=ApolloLinks.from([|authLink, httpLink|]),
    ~cache=inMemoryCache,
    (),
  );
