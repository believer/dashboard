let component = ReasonReact.statelessComponent("Notifications");

let make =
    (
      ~hasConfig,
      ~missing: ReasonReact.reactElement,
      ~result: ReasonApolloTypes.queryResponse('a),
      ~render: 'b => ReasonReact.reactElement,
      _children,
    ) => {
  ...component,
  render: _self =>
    hasConfig ?
      switch (result) {
      | Loading => <CardLoading />
      | Error(error) => <CardError errorMessage=error##message />
      | Data(response) => render(response)
      } :
      missing,
};
