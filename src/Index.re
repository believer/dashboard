ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client=Client.instance>
    <Router />
  </ReasonApollo.Provider>,
  "root",
);
ReasonReact.Router.push("");
