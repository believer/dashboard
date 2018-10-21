let component = ReasonReact.statelessComponent("Navigation");

let make = _children => {
  ...component,
  render: _self =>
    <div>
      <div className="bg-blue white pa4 flex justify-center">
        <div className="bt bw1 b--dashed w2" />
      </div>
      <div
        className="flex flex-column items-center bg-alabaster br h-100 b--black-10 pa4">
        <IconHome
          className="mb4"
          onClick={_ => ReasonReact.Router.push("/")}
        />
        <IconSettings onClick={_ => ReasonReact.Router.push("/#/settings")} />
      </div>
    </div>,
};
