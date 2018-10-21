let component = ReasonReact.statelessComponent("Link");

let make = (~href, children) => {
  ...component,
  render: _self =>
    <a
      className="link navy hover-hot-pink"
      href
      onClick={
        e => {
          e->ReactEvent.Mouse.preventDefault;
          ReasonReact.Router.push(href);
        }
      }>
      ...children
    </a>,
};
